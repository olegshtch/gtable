#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/notebook.h>
#include <gtkmm/actiongroup.h>
//#include <gtkmm/uimanager.h>
#include <gtkmm/builder.h>
#include "db/db.h"

class MainWindow : public Gtk::Window
{
public:
	MainWindow(GtkWindow *cobject, const Glib::RefPtr<Gtk::Builder>& builder);
	virtual ~MainWindow();
private:
	void OnNew();
	void OnOpen();
	void OnQuit();
	void OnAbout();

	void OnRun();
	void OnEdit();

	void ShowAllEntities();

	//Gtk::VBox m_Box;
	//Gtk::Statusbar m_Statusbar;
	//Glib::RefPtr<Gtk::Notebook> m_refNotebook;
	Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;
	//Glib::RefPtr<Gtk::UIManager> m_refUIManager;

	const Glib::RefPtr<Gtk::Builder>& m_refBuilder;

	DB::DataBase *m_DB;
};

#endif

