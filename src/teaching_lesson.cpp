#include "teaching_lesson.h"

TeachingLesson::LessonColumnRecord TeachingLesson::s_LessonColumnRecord;

TeachingLesson::TeachingLesson(GtkTreeView *cobject, const Glib::RefPtr<Gtk::Builder>& builder)
	:Glib::ObjectBase(typeid(TeachingLesson)),
	Gtk::TreeView(cobject),
	m_Model(ORM::Data::create(s_LessonColumnRecord)),
	m_TeacherColumn(_("teacher")),
	m_TeacherRenderer(s_LessonColumnRecord.fTeacher, DB::g_ModelTeachers, ORM::Expr<Glib::ustring>(ORM::Expr<Glib::ustring>(DB::g_ModelTeachers.secondname) + " " + DB::g_ModelTeachers.firstname + " " + DB::g_ModelTeachers.thirdname)),
	m_StreamColumn(_("stream"))
{
	set_model(m_Model);
	append_column(_("id"), s_LessonColumnRecord.fId);
	append_column(_("lesson"), s_LessonColumnRecord.fLesson);
	//append_column(_("stream"), m_LessonColumnRecord.fStream);
	
	m_TeacherColumn.pack_start(m_TeacherRenderer);
	m_TeacherRenderer.signal_edited().connect(sigc::mem_fun(*this, &TeachingLesson::OnTeacherEdited));
	Gtk::TreeViewColumn::SlotCellData slot = sigc::ptr_fun(CellRendererForeign::ForeignKeyAsString);
	m_TeacherColumn.set_cell_data_func(m_TeacherRenderer, slot);
	append_column(m_TeacherColumn);

	m_StreamRenderer.property_editable() = false;
	m_StreamColumn.pack_start(m_StreamRenderer);
	Gtk::TreeViewColumn::SlotCellData slot1 = sigc::ptr_fun(TeachingLesson::LessonAsStream);
	m_StreamColumn.set_cell_data_func(m_StreamRenderer, slot1);
	append_column(m_StreamColumn);

	m_Menu.accelerate(*this);
}

void TeachingLesson::SetSubgroup(long int id_subgroup)
{
	m_IdSubgroup = id_subgroup;
	DB::DataBase::Instance().GetLessonsForSubgroup(m_Model, id_subgroup);
}

void TeachingLesson::OnTeacherEdited(const Glib::ustring& path, long int id)
{
	std::cout << "TeachingLesson::OnTeacherEdited" << std::endl;
	Gtk::TreeIter iter = m_Model->get_iter(path);
	if(iter)
	{
		DB::DataBase::Instance().SetLessonTeacher(iter->get_value(s_LessonColumnRecord.fId), id);
	}
	DB::DataBase::Instance().GetLessonsForSubgroup(m_Model, m_IdSubgroup);
}

bool TeachingLesson::on_button_release_event(GdkEventButton *event)
{
	bool res = Gtk::TreeView::on_button_release_event(event);
	if(event->button == 3)
	{
		Gtk::TreeIter iter = get_selection()->get_selected();
		if(iter)
		{
			bool b_items = false;
			// generate menu
			m_Menu.items().erase(m_Menu.items().begin(), m_Menu.items().end());
			// menu for adding
			Glib::RefPtr<ORM::Data> data = ORM::Data::create(DB::g_IdTextScheme);
			DB::DataBase::Instance().GetLessonsListForAdding(data, m_IdSubgroup, iter->get_value(s_LessonColumnRecord.fId));
			if(data->children().size() > 0)
			{
				Gtk::MenuItem *add_item = Gtk::manage(new Gtk::MenuItem(_("Add to stream")));
				m_Menu.append(*add_item);
				Gtk::Menu *streams_menu = Gtk::manage(new Gtk::Menu());
				add_item->set_submenu(*streams_menu);
				for(Gtk::TreeIter stream_iter = data->children().begin(); stream_iter != data->children().end(); ++ stream_iter)
				{
					Gtk::MenuItem *item = Gtk::manage(new Gtk::MenuItem(stream_iter->get_value(DB::g_IdTextScheme.fText)));
					streams_menu->append(*item);
					item->signal_activate().connect(sigc::bind(sigc::mem_fun(*this, &TeachingLesson::OnAddToStreamActivate), stream_iter->get_value(DB::g_IdTextScheme.fId), iter->get_value(s_LessonColumnRecord.fId)));
					//item->show();
				}
				//add_item->show();
				b_items = true;

			}
			// menu for removing
			if(DB::DataBase::Instance().IsGroupsInStream(iter->get_value(s_LessonColumnRecord.fId), m_IdSubgroup))
			{
				Gtk::MenuItem *del_item = Gtk::manage(new Gtk::MenuItem(_("Delete from stream")));
				m_Menu.append(*del_item);
				del_item->signal_activate().connect(sigc::bind(sigc::mem_fun(*this, &TeachingLesson::OnDelFromStreamActivate), iter->get_value(s_LessonColumnRecord.fId)));
				b_items = true;
			}
			if(b_items)
			{
				m_Menu.show_all_children();
				m_Menu.popup(0, event->time);
			}
		}
	}
	return res;
}

void TeachingLesson::OnAddToStreamActivate(long int id_lesson_to, long int id_lesson_from)
{
	std::cout << "TeachingLesson::OnAddToStreamActivate id_lesson_to=" << id_lesson_to << " id_lesson_from=" << id_lesson_from << std::endl;
	DB::DataBase::Instance().MoveLessons(id_lesson_from, id_lesson_to);
	DB::DataBase::Instance().GetLessonsForSubgroup(m_Model, m_IdSubgroup);
}

void TeachingLesson::OnDelFromStreamActivate(long int id_stream)
{
	DB::DataBase::Instance().RemoveSubgroupFromLesson(id_stream, m_IdSubgroup);
	DB::DataBase::Instance().GetLessonsForSubgroup(m_Model, m_IdSubgroup);
}

void TeachingLesson::LessonAsStream(Gtk::CellRenderer *cell, const Gtk::TreeModel::iterator& row)
{
	Gtk::CellRendererText* renderer = reinterpret_cast<Gtk::CellRendererText*>(cell);
	long int id_lesson = row->get_value(s_LessonColumnRecord.fId);
	renderer->property_text() = DB::DataBase::Instance().GetStreamByLesson(id_lesson);
}

