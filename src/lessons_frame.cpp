#include <iostream>
#include "lessons_frame.h"
#include "db/models.h"
#include "shared.h"
#include "order_dialog.h"

LessonsFrame::LessonsFrame(DB::DataBase &db)
	:m_DB(db), m_LessonsList(db, DB::g_Lessons), m_AuditoriumsList(db, DB::g_LessonsAuditoriums), m_ButtonOrder(_("Edit order"))
{
	m_ButtonOrder.signal_clicked().connect(sigc::mem_fun(*this, &LessonsFrame::OnOrder));

	m_LessonsList.GetButtonBox()->pack_start(m_ButtonOrder, Gtk::PACK_SHRINK);

	pack1(m_LessonsList);
	pack2(m_AuditoriumsList);

	m_LessonsList.GetListWidget()->signal_cursor_changed().connect(sigc::mem_fun(*this, &LessonsFrame::OnChoosed));

	show_all_children();
}

void LessonsFrame::OnChoosed()
{
	m_AuditoriumsList.SetParentEntityId(m_LessonsList.GetListWidget()->get_selection()->get_selected()->get_value(DB::g_ModelEntity.id));
}

void LessonsFrame::OnOrder()
{
	OrderDialog dialog(m_DB, DB::g_OrderLessons);
	dialog.run();
}


