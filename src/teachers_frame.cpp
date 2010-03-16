#include <iostream>
#include "teachers_frame.h"
#include "db/models.h"

TeachersFrame::TeachersFrame(DB::DataBase &db)
	:m_TeachersList(db, DB::g_Teachers), m_LessonsList(db, DB::g_TeachersLessons)
{
	pack1(m_TeachersList);
	pack2(m_LessonsList);

	m_TeachersList.GetListWidget()->signal_cursor_changed().connect(sigc::mem_fun(*this, &TeachersFrame::OnChoosed));

	show_all_children();
}

void TeachersFrame::OnChoosed()
{
	m_LessonsList.SetParentEntityId(m_TeachersList.GetListWidget()->get_selection()->get_selected()->get_value(DB::g_ModelEntity.id));
}

