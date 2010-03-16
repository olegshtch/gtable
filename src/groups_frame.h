#ifndef _GROUPS_FRAME_H_
#define _GROUPS_FRAME_H_

#include "entities_list_widget.h"

class GroupsFrame : public EntitiesListWidget
{
public:
	GroupsFrame(DB::DataBase &db)
		:EntitiesListWidget(db, DB::g_Groups) {show_all_children();}
	~GroupsFrame() {}
private:
};

#endif

