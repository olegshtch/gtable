#ifndef _TEACHERS_FRAME_H_
#define _TEACHERS_FRAME_H_

#include <gtkmm/paned.h>
#include "entities_list_widget.h"
#include "link_list_widget.h"

class TeachersFrame : public Gtk::VPaned
{
public:
	TeachersFrame(DB::DataBase &db);
	~TeachersFrame() {}
private:
	EntitiesListWidget m_TeachersList;
	LinkListWidget m_LessonsList;

	void OnChoosed();
};

#endif

