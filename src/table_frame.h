#ifndef _TABLE_FRAME_H_
#define _TABLE_FRAME_H_

#include <gtkmm/box.h>
#include <gtkmm/table.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/menu.h>
#include "db/db.h"
#include "table_cell.h"

class TableFrame : public Gtk::HBox
{
public:
	TableFrame(DB::DataBase &db);
	~TableFrame() {}
	Gtk::Menu& GetPopupMenu() {return m_PopupMenu;}
	void SetCell(TableCell *cell) {m_CellChoosed = cell;}
private:
	void OnChoose();
	void OnSet();
	void OnDrop();

	DB::DataBase &m_DB;
	Gtk::Table m_Table;
	Gtk::VBox m_UnsetLessons;
	Glib::RefPtr<ORM::Table> m_Auditoriums;
	Glib::RefPtr<ORM::Table> m_Days;
	Glib::RefPtr<ORM::Table> m_Hours;
	Gtk::ScrolledWindow m_SWTable, m_SWUnsetLessons;
	Glib::RefPtr<ORM::Table> m_LessonRecords;
	Gtk::Menu m_PopupMenu;
	Gtk::MenuItem m_MenuChoose, m_MenuSet, m_MenuDrop;
	TableCell *m_CellFrom, *m_CellTo, *m_CellChoosed;
};

#endif

