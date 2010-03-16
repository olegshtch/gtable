#ifndef _TIME_FRAME_H_
#define _TIME_FRAME_H_

#include <gtkmm/paned.h>
#include "entities_list_widget.h"

class TimeFrame : public Gtk::HPaned
{
public:
	TimeFrame(DB::DataBase &db);
	~TimeFrame();
private:
	EntitiesListWidget m_DaysList, m_HoursList;
};

#endif

