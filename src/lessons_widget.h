#ifndef _LESSONS_WIDGET_H_
#define _LESSONS_WIDGET_H_

#include <gtkmm/box.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/listviewtext.h>
#include <gtkmm/button.h>
#include <gtkmm/scrolledwindow.h>
#include "db/db.h"

class LessonsWidget : public Gtk::HBox
{
public:
	LessonsWidget(DB::DataBase &db, const DB::Link_TeachPlan &link);
	~LessonsWidget() {}
	void Refresh();
	void SetParentEntityId(int id) {m_ParentId=id;Refresh();}
private:
	DB::DataBase &m_DB;
	const DB::Link_TeachPlan &m_Link;
	int m_ParentId;
	bool m_Recurse;

	Glib::RefPtr<ORM::Table> m_Model;
	Gtk::VButtonBox m_ButtonBox;
	Gtk::TreeView m_ListBox;
	Gtk::Button m_Buttons[2];
	Gtk::ScrolledWindow m_Scrolled;

	void OnAppend();
	void OnDelete();
	void OnEditRow(const Gtk::TreeModel::Path& path, const Gtk::TreeModel::iterator& iter);
};

#endif

