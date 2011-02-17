#ifndef _ORDER_DIALOG_H_
#define _ORDER_DIALOG_H_

#include <gtkmm/dialog.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/button.h>
#include <gtkmm/listviewtext.h>
#include <gtkmm/scrolledwindow.h>
#include "db/db.h"

class OrderDialog : public Gtk::Dialog
{
public:
	OrderDialog(DB::DataBase &db, const DB::Link_N2N &link);
	~OrderDialog();
private:
	DB::DataBase &m_DB;
	const DB::Link_N2N &m_Link;
	Glib::RefPtr<ORM::Data> m_Model;

	Gtk::HButtonBox m_ButtonBox;
	Gtk::Button m_Buttons[2];
	Gtk::TreeView m_List;
	Gtk::ScrolledWindow m_Scrolled;

	void OnAppend();
	void OnDelete();
	void Refresh();
};

#endif

