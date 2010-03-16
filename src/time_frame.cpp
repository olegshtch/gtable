#include "time_frame.h"
#include "db/entities.h"

TimeFrame::TimeFrame(DB::DataBase &db)
	:m_DaysList(db, DB::g_Days), m_HoursList(db, DB::g_Hours)
{
	pack1(m_DaysList);
	pack2(m_HoursList);
	show_all_children();
}

TimeFrame::~TimeFrame()
{
}


