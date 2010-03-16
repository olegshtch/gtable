#include "auditorium_frame.h"
#include "db/entities.h"
#include "db/models.h"

AuditoriumFrame::AuditoriumFrame(DB::DataBase &db)
	:EntitiesListWidget(db, DB::g_Auditoriums)
{
	show_all_children();
}

AuditoriumFrame::~AuditoriumFrame()
{
}


