#include <gtkmm/stock.h>
#include <gtkmm/filechooserdialog.h>
#include <iostream>
#include "main_window.h"
#include "shared.h"
#include "ga/ga.h"
#include "orm/data.h"
#include "db/models.h"

MainWindow::MainWindow(GtkWindow *cobject, const Glib::RefPtr<Gtk::Builder>& builder)
	:Gtk::Window(cobject),
	m_refModelHours(ORM::Data::create(DB::g_ModelHours)),
	m_refBuilder(builder),
	m_DB(NULL)
{
	m_refActionGroup = Glib::RefPtr<Gtk::ActionGroup>::cast_dynamic(m_refBuilder->get_object("ActionGroup"));

	m_refActionGroup->add(Glib::RefPtr<Gtk::Action>::cast_dynamic(m_refBuilder->get_object("FileNew")),sigc::mem_fun(*this, &MainWindow::OnNew));
	m_refActionGroup->add(Glib::RefPtr<Gtk::Action>::cast_dynamic(m_refBuilder->get_object("FileOpen")),sigc::mem_fun(*this, &MainWindow::OnOpen));
	m_refActionGroup->add(Glib::RefPtr<Gtk::Action>::cast_dynamic(m_refBuilder->get_object("FileQuit")),sigc::mem_fun(*this, &MainWindow::OnQuit));
	
	m_refActionGroup->add(Glib::RefPtr<Gtk::Action>::cast_dynamic(m_refBuilder->get_object("RunSolve")),sigc::mem_fun(*this, &MainWindow::OnRun));
	m_refActionGroup->add(Glib::RefPtr<Gtk::Action>::cast_dynamic(m_refBuilder->get_object("RunEdit")),sigc::mem_fun(*this, &MainWindow::OnEdit));

	m_refActionGroup->add(Glib::RefPtr<Gtk::Action>::cast_dynamic(m_refBuilder->get_object("HelpAbout")),sigc::mem_fun(*this, &MainWindow::OnAbout));

	m_refActionGroup->add(Glib::RefPtr<Gtk::Action>::cast_dynamic(m_refBuilder->get_object("ActionAppend")),sigc::mem_fun(*this, &MainWindow::OnAppend));

	// connect models
	
	Gtk::TreeView* m_pTreeView = 0;
	m_refBuilder->get_widget("TreeViewHours", m_pTreeView);
	if(! m_pTreeView)
	{
		throw Glib::Error(1, 0, "Cann't load TreeViewHours");
	}
	m_pTreeView->set_can_focus(true);
	m_pTreeView->set_model(m_refModelHours);
	m_pTreeView->append_column(_("id"), DB::g_ModelHours.fId);
	m_pTreeView->append_column(_("name"), DB::g_ModelHours.name);
	m_pTreeView->set_headers_visible(true);

	show_all_children();
}

MainWindow::~MainWindow()
{
	if(m_DB)
	{
		delete m_DB;
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
	Glib::RefPtr<Gtk::Dialog> ref_dialog = Glib::RefPtr<Gtk::Dialog>::cast_dynamic(m_refBuilder->get_object("AboutDialog"));
	ref_dialog->run();
}

void MainWindow::OnRun()
{
	std::cout << "run" << std::endl;
	if(m_DB)
	{
		GA ga(*m_DB);
		ga.Run();
	}
}

void MainWindow::OnEdit()
{
	show_all_children();
}

void MainWindow::OnAppend()
{
	m_DB->AppendEntity(DB::g_ModelHours, "");
}

void MainWindow::ShowAllEntities()
{
	m_DB->ListEntity(DB::g_ModelHours, m_refModelHours, false);
	show_all_children();
}



