#include <gtkmm/stock.h>
#include <gtkmm/filechooserdialog.h>
#include <iostream>
#include "main_window.h"
#include "shared.h"
#include "notebook_label_widget.h"
#include "time_frame.h"
#include "auditorium_frame.h"
#include "teachers_frame.h"
#include "lessons_frame.h"
#include "groups_frame.h"
#include "table_frame.h"

MainWindow::MainWindow()
	:m_DB(0), m_Conditions(0)
{
	set_title(_("Table"));

	m_refActionGroup = Gtk::ActionGroup::create();
	m_refActionGroup->add(Gtk::Action::create("MenuFile", _("File")));
	m_refActionGroup->add(Gtk::Action::create("FileNew", Gtk::Stock::NEW),
		sigc::mem_fun(*this, &MainWindow::OnNew));
	m_refActionGroup->add(Gtk::Action::create("FileOpen", Gtk::Stock::OPEN),
		sigc::mem_fun(*this, &MainWindow::OnOpen));
	m_refActionGroup->add(Gtk::Action::create("FileQuit", Gtk::Stock::QUIT),
		sigc::mem_fun(*this, &MainWindow::OnQuit));
	m_refActionGroup->add(Gtk::Action::create("MenuRun", _("Run")));
	m_refActionGroup->add(Gtk::Action::create("RunSolve", Gtk::Stock::GO_FORWARD),
		sigc::mem_fun(*this, &MainWindow::OnRun));
	m_refActionGroup->add(Gtk::Action::create("RunEdit", Gtk::Stock::EDIT),
		sigc::mem_fun(*this, &MainWindow::OnEdit));
	m_refActionGroup->add(Gtk::Action::create("MenuHelp", _("Help")));
	m_refActionGroup->add(Gtk::Action::create("HelpAbout", Gtk::Stock::ABOUT),
		sigc::mem_fun(*this, &MainWindow::OnAbout));


	m_refUIManager = Gtk::UIManager::create();
	m_refUIManager->insert_action_group(m_refActionGroup);
	add_accel_group(m_refUIManager->get_accel_group());

	Glib::ustring ui_info = 
		"<ui>\
		 <menubar name='MenuBar'>\
		  <menu action='MenuFile'>\
		   <menuitem action='FileNew' />\
		   <menuitem action='FileOpen' />\
		   <separator />\
		   <menuitem action='FileQuit' />\
		  </menu>\
		  <menu action='MenuRun'>\
		   <menuitem action='RunSolve' />\
		   <menuitem action='RunEdit' />\
		  </menu>\
		  <menu action='MenuHelp'>\
		   <menuitem action='HelpAbout' />\
		  </menu>\
		 </menubar>\
		 <toolbar name='ToolBar'>\
		  <toolitem action='FileNew' />\
		  <toolitem action='FileOpen' />\
		  <toolitem action='RunEdit' />\
		  <toolitem action='HelpAbout' />\
		  <toolitem action='FileQuit' />\
		 </toolbar>\
		</ui>";

#ifdef GLIBMM_EXCEPTIONS_ENABLED
	try
	{
		m_refUIManager->add_ui_from_string(ui_info);
	}
	catch(const Glib::Error &e)
	{
		std::cerr << e.what() << std::endl;
	}
#else
	std::auto_ptr<Glib::Error> ex;
	m_refUIManager->add_ui_from_string(ui_info, ex);
	if(ex.get())
	{
		std::cerr << ex->what() << std::endl;
	}
#endif // GLIBMM_EXCEPTIONS_ENABLED

	Gtk::Widget* pMenubar = m_refUIManager->get_widget("/MenuBar");
	if(pMenubar)
	{
		m_Box.pack_start(*pMenubar, Gtk::PACK_SHRINK);
	}
	Gtk::Widget* pToolbar = m_refUIManager->get_widget("/ToolBar");
	if(pToolbar)
	{
		m_Box.pack_start(*pToolbar, Gtk::PACK_SHRINK);
	}

	m_Box.pack_start(m_Notebook, Gtk::PACK_EXPAND_WIDGET);
	m_Box.pack_start(m_Statusbar, Gtk::PACK_SHRINK);

	add(m_Box);
	show_all_children();
}

MainWindow::~MainWindow()
{
	if(m_DB)
	{
		delete m_DB;
	}
	if(m_Conditions)
	{
		delete m_Conditions;
	}
}

void MainWindow::OnNew()
{
	Gtk::FileChooserDialog dialog(*this,_("Choose file for saving database:"),
		Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialog.add_button(Gtk::Stock::CANCEL,Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::SAVE,Gtk::RESPONSE_YES);
	if(dialog.run()==Gtk::RESPONSE_YES)
	{
		if(m_DB)
		{
			delete m_DB;
			m_DB = 0;
		}
		if(m_Conditions)
		{
			delete m_Conditions;
			m_Conditions = 0;
		}
		m_DB = new DB::DataBase(dialog.get_filename(), true);
		ShowAllEntities();
	}
}

void MainWindow::OnOpen()
{
	Gtk::FileChooserDialog dialog(*this,_("Choose file for opening database:"),
		Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialog.add_button(Gtk::Stock::CANCEL,Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::OPEN,Gtk::RESPONSE_YES);
	if(dialog.run()==Gtk::RESPONSE_YES)
	{
		if(m_DB)
		{
			delete m_DB;
			m_DB = 0;
		}
		if(m_Conditions)
		{
			delete m_Conditions;
			m_Conditions = 0;
		}
		m_DB = new DB::DataBase(dialog.get_filename(), false);
		ShowAllEntities();
	}
}

void MainWindow::OnQuit()
{
	hide();
}

void MainWindow::OnAbout()
{
}

void MainWindow::OnRun()
{
	std::cout << "run" << std::endl;
	if(m_Conditions)
	{
		delete m_Conditions;
		m_Conditions = 0;
	}
	m_Conditions = new Conditions(*m_DB);
}

void MainWindow::OnEdit()
{
	m_Notebook.append_page(*Gtk::manage(new TableFrame(*m_DB)), *Gtk::manage(new NotebookLabelWidget(_("Table"))));
}

void MainWindow::ShowAllEntities()
{
	m_Notebook.append_page(*Gtk::manage(new TimeFrame(*m_DB)), *Gtk::manage(new NotebookLabelWidget(_("Time"))));
	m_Notebook.append_page(*Gtk::manage(new AuditoriumFrame(*m_DB)), *Gtk::manage(new NotebookLabelWidget(_("Auditoruiums"))));
	m_Notebook.append_page(*Gtk::manage(new TeachersFrame(*m_DB)), *Gtk::manage(new NotebookLabelWidget(_("Teachers"))));
	m_Notebook.append_page(*Gtk::manage(new LessonsFrame(*m_DB)), *Gtk::manage(new NotebookLabelWidget(_("Lessons"))));
	m_Notebook.append_page(*Gtk::manage(new GroupsFrame(*m_DB)), *Gtk::manage(new NotebookLabelWidget(_("Groups"))));
	show_all_children();
}



