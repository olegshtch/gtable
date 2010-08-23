#ifndef _TABLE_FRAME_H_
#define _TABLE_FRAME_H_

#include <gtkmm/box.h>
#include <gtkmm/table.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/menu.h>
#include "db/db.h"

class TableFrame : public Gtk::HBox
{
public:
	TableFrame(DB::DataBase &db);
	~TableFrame() {}
private:
	DB::DataBase &m_DB;
	Gtk::VBox m_UnsetLessons;
	Glib::RefPtr<ORM::Table> m_Auditoriums;
	Glib::RefPtr<ORM::Table> m_Days;
	Glib::RefPtr<ORM::Table> m_Hours;
	Gtk::ScrolledWindow m_SWTable, m_SWUnsetLessons;
	Glib::RefPtr<ORM::Table> m_LessonRecords;
};

#endif

