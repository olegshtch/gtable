#ifndef _GROUPS_FRAME_H_
#define _GROUPS_FRAME_H_

#include <gtkmm/paned.h>
#include "entities_list_widget.h"
#include "lessons_widget.h"

class GroupsFrame : public Gtk::VPaned
{
public:
	GroupsFrame(DB::DataBase &db);
	~GroupsFrame() {}
private:
	EntitiesListWidget m_GroupsList;
	LessonsWidget m_LessonsList;

	void OnChoose();
	
};

#endif

