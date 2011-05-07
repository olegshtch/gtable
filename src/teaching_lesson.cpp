#include "teaching_lesson.h"

TeachingLesson::TeachingLesson(GtkTreeView *cobject, const Glib::RefPtr<Gtk::Builder>& builder)
	:Glib::ObjectBase(typeid(TeachingLesson)),
	Gtk::TreeView(cobject),
	m_Model(ORM::Data::create(m_LessonColumnRecord)),
	m_TeacherColumn(_("teacher")),
	m_TeacherRenderer(m_LessonColumnRecord.fTeacher, DB::g_ModelTeachers, ORM::Expr<Glib::ustring>(ORM::Expr<Glib::ustring>(DB::g_ModelTeachers.secondname) + " " + DB::g_ModelTeachers.firstname + " " + DB::g_ModelTeachers.thirdname))
{
	set_model(m_Model);
	append_column(_("id"), m_LessonColumnRecord.fId);
	append_column(_("lesson"), m_LessonColumnRecord.fLesson);
	
	m_TeacherColumn.pack_start(m_TeacherRenderer);
	m_TeacherRenderer.signal_edited().connect(sigc::mem_fun(*this, &TeachingLesson::OnTeacherEdited));
	Gtk::TreeViewColumn::SlotCellData slot = sigc::ptr_fun(CellRendererForeign::ForeignKeyAsString);

	m_TeacherColumn.set_cell_data_func(m_TeacherRenderer, slot);
	append_column(m_TeacherColumn);

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
		DB::DataBase::Instance().SetLessonTeacher(iter->get_value(m_LessonColumnRecord.fId), id);
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
			// generate menu
			m_Menu.items().erase(m_Menu.items().begin(), m_Menu.items().end());
			// menu for adding
			Glib::RefPtr<ORM::Data> data = ORM::Data::create(DB::g_IdTextScheme);
			DB::DataBase::Instance().GetLessonsListForAdding(data, m_IdSubgroup, iter->get_value(m_LessonColumnRecord.fId));
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
					item->signal_activate().connect(sigc::bind(sigc::mem_fun(*this, &TeachingLesson::OnAddToStreamActivate), stream_iter->get_value(DB::g_IdTextScheme.fId), iter->get_value(m_LessonColumnRecord.fId)));
					item->show();
				}
				add_item->show();
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

