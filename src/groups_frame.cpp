#include "groups_frame.h"
#include "db/models.h"

GroupsFrame::GroupsFrame(DB::DataBase &db)
	:m_GroupsList(db, DB::g_Groups), m_LessonsList(db, DB::g_TeachPlan)
{
	pack1(m_GroupsList);
	pack2(m_LessonsList);

	m_GroupsList.GetListWidget()->signal_cursor_changed().connect(sigc::mem_fun(*this, &GroupsFrame::OnChoose));
	
	show_all_children();
}

void GroupsFrame::OnChoose()
{
	m_LessonsList.SetParentEntityId(m_GroupsList.GetListWidget()->get_selection()->get_selected()->get_value(DB::g_ModelEntity.id));
}

