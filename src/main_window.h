#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/notebook.h>
#include <gtkmm/actiongroup.h>
#include <gtkmm/uimanager.h>
#include "db/db.h"

class MainWindow : public Gtk::Window
{
public:
	MainWindow();
	~MainWindow();
private:
	void OnNew();
	void OnOpen();
	void OnQuit();
	void OnAbout();

	void ShowAllEntities();

	Gtk::VBox m_Box;
	Gtk::Statusbar m_Statusbar;
	Gtk::Notebook m_Notebook;
	Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;
	Glib::RefPtr<Gtk::UIManager> m_refUIManager;

	DB::DataBase *m_DB;
};

#endif

