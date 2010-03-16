#ifndef _ENTITIES_LIST_WIDGET_H_
#define _ENTITIES_LIST_WIDGET_H_

#include <gtkmm/box.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/listviewtext.h>
#include <gtkmm/button.h>
#include <gtkmm/scrolledwindow.h>
#include "db/db.h"

class EntitiesListWidget : public Gtk::HBox
{
public:
	EntitiesListWidget(DB::DataBase &db, const DB::Entity &ent);
	~EntitiesListWidget();
	void Refresh();
	Gtk::TreeView* GetListWidget() {return &m_ListBox;}
	Gtk::ButtonBox* GetButtonBox() {return &m_ButtonBox;}
private:
	DB::DataBase &m_DB;
	const DB::Entity &m_Entity;
	bool m_IsEditing;
	
	Glib::RefPtr<Gtk::ListStore> m_Model;
	Gtk::VButtonBox m_ButtonBox;
	Gtk::TreeView m_ListBox;
	Gtk::Button m_Buttons[3];
	Gtk::ScrolledWindow m_Scrolled;

	void OnAppend();
	void OnDelete();
	void OnEdit();
	void OnRowEdited(const Gtk::TreeModel::Path& path, const Gtk::TreeModel::iterator& iter);
};

#endif

