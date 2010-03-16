#ifndef _LESSONS_FRAME_H_
#define _LESSONS_FRAME_H_

#include <gtkmm/paned.h>
#include "entities_list_widget.h"
#include "link_list_widget.h"

class LessonsFrame : public Gtk::VPaned
{
public:
	LessonsFrame(DB::DataBase &db);
	~LessonsFrame() {}
private:
	DB::DataBase &m_DB;
	EntitiesListWidget m_LessonsList;
	LinkListWidget m_AuditoriumsList;

	Gtk::Button m_ButtonOrder;

	void OnChoosed();
	void OnOrder();
};

#endif

