#ifndef _LINK_LIST_WIDGET_H_
#define _LINK_LIST_WIDGET_H_

#include <gtkmm/box.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/listviewtext.h>
#include <gtkmm/button.h>
#include <gtkmm/scrolledwindow.h>
#include "db/db.h"

class LinkListWidget : public Gtk::HBox
{
public:
	LinkListWidget(DB::DataBase &db, const DB::Link_N2N &link);
	~LinkListWidget();
	void Refresh();
	Gtk::TreeView* GetListWidget() {return &m_ListBox;}
	Gtk::ButtonBox* GetButtonBox() {return &m_ButtonBox;}
	void SetParentEntityId(int id) {m_ParentId=id;Refresh();}
private:
	DB::DataBase &m_DB;
	const DB::Link_N2N &m_Link;
	int m_ParentId;
	
	Glib::RefPtr<ORM::Table> m_Model;
	Gtk::VButtonBox m_ButtonBox;
	Gtk::TreeView m_ListBox;
	Gtk::Button m_Buttons[2];
	Gtk::ScrolledWindow m_Scrolled;

	void OnAppend();
	void OnDelete();
};

#endif

