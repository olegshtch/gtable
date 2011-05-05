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
	append_column(_("stream"), m_LessonColumnRecord.fStream);
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
			DB::DataBase::Instance().GetStreamsListForAdding(data, m_IdSubgroup, iter->get_value(m_LessonColumnRecord.fStream));

			m_Menu.popup(0, event->time);
		}
	}
	return res;
}

