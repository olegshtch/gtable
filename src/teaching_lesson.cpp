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
}

void TeachingLesson::SetSubgroup(long int id_subgroup)
{
	DB::DataBase::Instance().GetLessonsForSubgroup(m_Model, id_subgroup);
}

void TeachingLesson::OnTeacherEdited(const Glib::ustring& path, long int id)
{
	Gtk::TreeIter iter = m_Model->get_iter(path);
	if(iter)
	{
		DB::DataBase::Instance().SetLessonsTeacher(iter->get_value(m_LessonColumnRecord.fId), id);
	}
}

