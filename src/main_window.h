#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/notebook.h>
#include <gtkmm/actiongroup.h>
#include <gtkmm/builder.h>
#include <gtkmm/comboboxentry.h>
#include "cellrendererforeign.h"
#include "listview.h"
#include "db/db.h"

class MainWindow : public Gtk::Window
{
public:
	MainWindow(GtkWindow *cobject, const Glib::RefPtr<Gtk::Builder>& builder);
	virtual ~MainWindow();
private:
	Glib::RefPtr<Gtk::Builder> m_refBuilder;

	void OnNew();
	void OnOpen();
	void OnSave();
	void OnQuit();
	void OnAbout();

	void OnRun();
	void OnEdit();

	void OnAppend();
	void OnDelete();

	void ShowAllEntities();

	bool OnFocusIn(GdkEventFocus* event, ListView *list_view);
	bool OnFocusOut(GdkEventFocus* event);

	ListView* AddListView(const Glib::ustring& name, const ORM::Table& scheme);

	// Loadings -> Holydays
	Gtk::ComboBoxEntry *m_HolydaysCategory;
	Gtk::ComboBoxEntry *m_HolydaysObjectList;
	void SwitchHolydayCategory();

	Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;

	ListView *m_pCurrentListView;

	ComboScheme m_ComboScheme;
};

#endif

