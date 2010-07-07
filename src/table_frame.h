#ifndef _TABLE_FRAME_H_
#define _TABLE_FRAME_H_

#include <gtkmm/box.h>
#include "db/db.h"

class TableFrame : public Gtk::VBox
{
public:
	TableFrame(DB::DataBase &db);
	~TableFrame() {}
private:
};

#endif

