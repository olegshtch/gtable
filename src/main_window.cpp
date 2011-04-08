#include <gtkmm/stock.h>
#include <gtkmm/filechooserdialog.h>
#include <iostream>
#include "main_window.h"
#include "shared.h"
#include "sheet.h"
#include "ga/ga.h"
#include "orm/data.h"
#include "db/models.h"

MainWindow::MainWindow(GtkWindow *cobject, const Glib::RefPtr<Gtk::Builder>& builder)
	:Gtk::Window(cobject),
	m_refBuilder(builder),
	m_pCurrentListView(NULL)
{
	m_refActionGroup = Glib::RefPtr<Gtk::ActionGroup>::cast_dynamic(m_refBuilder->get_object("ActionGroup"));

	m_refActionGroup->add(Glib::RefPtr<Gtk::Action>::cast_dynamic(m_refBuilder->get_object("FileNew")),sigc::mem_fun(*this, &MainWindow::OnNew));
	m_refActionGroup->add(Glib::RefPtr<Gtk::Action>::cast_dynamic(m_refBuilder->get_object("FileOpen")),sigc::mem_fun(*this, &MainWindow::OnOpen));
	m_refActionGroup->add(Glib::RefPtr<Gtk::Action>::cast_dynamic(m_refBuilder->get_object("FileSave")),sigc::mem_fun(*this, &MainWindow::OnSave));
	m_refActionGroup->add(Glib::RefPtr<Gtk::Action>::cast_dynamic(m_refBuilder->get_object("FileQuit")),sigc::mem_fun(*this, &MainWindow::OnQuit));
	
	m_refActionGroup->add(Glib::RefPtr<Gtk::Action>::cast_dynamic(m_refBuilder->get_object("RunSolve")),sigc::mem_fun(*this, &MainWindow::OnRun));
	m_refActionGroup->add(Glib::RefPtr<Gtk::Action>::cast_dynamic(m_refBuilder->get_object("RunEdit")),sigc::mem_fun(*this, &MainWindow::OnEdit));

	m_refActionGroup->add(Glib::RefPtr<Gtk::Action>::cast_dynamic(m_refBuilder->get_object("HelpAbout")),sigc::mem_fun(*this, &MainWindow::OnAbout));

	m_refActionGroup->add(Glib::RefPtr<Gtk::Action>::cast_dynamic(m_refBuilder->get_object("ActionAppend")),sigc::mem_fun(*this, &MainWindow::OnAppend));
	m_refActionGroup->add(Glib::RefPtr<Gtk::Action>::cast_dynamic(m_refBuilder->get_object("ActionDelete")),sigc::mem_fun(*this, &MainWindow::OnDelete));

	// connect models
	
	ListView* m_pTreeView = 0;
	m_refBuilder->get_widget_derived("TreeViewHours", m_pTreeView);
	if(! m_pTreeView)
	{
		throw Glib::Error(1, 0, "Cann't load TreeViewHours");
	}
	m_pTreeView->set_scheme(DB::g_ModelHours);
	m_pTreeView->append_column(_("id"), DB::g_ModelHours.fId);
	m_pTreeView->append_column_editable(_("start"), DB::g_ModelHours.start);
	m_pTreeView->append_column_editable(_("finish"), DB::g_ModelHours.finish);
	m_pTreeView->signal_focus_in_event().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::OnFocusIn), m_pTreeView));
	m_pTreeView->signal_focus_out_event().connect(sigc::mem_fun(*this, &MainWindow::OnFocusOut));

	m_refBuilder->get_widget_derived("TreeViewDays", m_pTreeView);
	if(! m_pTreeView)
	{
		throw Glib::Error(1, 0, "Cann't load TreeViewDays");
	}
	m_pTreeView->set_scheme(DB::g_ModelDays);
	m_pTreeView->append_column(_("id"), DB::g_ModelDays.fId);
	m_pTreeView->append_column_editable(_("name"), DB::g_ModelDays.name);
	m_pTreeView->signal_focus_in_event().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::OnFocusIn), m_pTreeView));
	m_pTreeView->signal_focus_out_event().connect(sigc::mem_fun(*this, &MainWindow::OnFocusOut));

	m_refBuilder->get_widget_derived("TreeViewFaculty", m_pTreeView);
	if(! m_pTreeView)
	{
		throw Glib::Error(1, 0, "Cann't load TreeViewFaculty");
	}
	m_pTreeView->set_scheme(DB::g_ModelFaculties);
	m_pTreeView->append_column(_("id"), DB::g_ModelFaculties.fId);
	m_pTreeView->append_column_editable(_("name"), DB::g_ModelFaculties.name);
	m_pTreeView->append_column_editable(_("abbreviation"), DB::g_ModelFaculties.abbr);
	m_pTreeView->signal_focus_in_event().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::OnFocusIn), m_pTreeView));
	m_pTreeView->signal_focus_out_event().connect(sigc::mem_fun(*this, &MainWindow::OnFocusOut));

	m_refBuilder->get_widget_derived("TreeViewChairs", m_pTreeView);
	if(! m_pTreeView)
	{
		throw Glib::Error(1, 0, "Cann't load TreeViewChairs");
	}
	m_pTreeView->set_scheme(DB::g_ModelChairs);
	m_pTreeView->append_column(_("id"), DB::g_ModelChairs.fId);
	m_pTreeView->append_column_editable(_("name"), DB::g_ModelChairs.name);
	m_pTreeView->append_column_editable(_("abbreviation"), DB::g_ModelChairs.abbr);
	m_pTreeView->append_column_foreign_editable(_("faculty"), DB::g_ModelChairs.faculty, DB::g_ModelFaculties, DB::g_ModelFaculties.abbr);
	m_pTreeView->signal_focus_in_event().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::OnFocusIn), m_pTreeView));
	m_pTreeView->signal_focus_out_event().connect(sigc::mem_fun(*this, &MainWindow::OnFocusOut));

	m_refBuilder->get_widget_derived("TreeViewTeachers", m_pTreeView);
	if(! m_pTreeView)
	{
		throw Glib::Error(1, 0, "Cann't load TreeViewTeachers");
	}
	m_pTreeView->set_scheme(DB::g_ModelTeachers);
	m_pTreeView->append_column(_("id"), DB::g_ModelTeachers.fId);
	m_pTreeView->append_column_editable(_("firstname"), DB::g_ModelTeachers.firstname);
	m_pTreeView->append_column_editable(_("secondname"), DB::g_ModelTeachers.secondname);
	m_pTreeView->append_column_editable(_("thirdname"), DB::g_ModelTeachers.thirdname);
	m_pTreeView->append_column_foreign_editable(_("chair"), DB::g_ModelTeachers.chair, DB::g_ModelChairs, DB::g_ModelChairs.abbr);
	m_pTreeView->signal_focus_in_event().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::OnFocusIn), m_pTreeView));
	m_pTreeView->signal_focus_out_event().connect(sigc::mem_fun(*this, &MainWindow::OnFocusOut));

	m_refBuilder->get_widget_derived("TreeViewSpecialities", m_pTreeView);
	if(! m_pTreeView)
	{
		throw Glib::Error(1, 0, "Cann't load TreeViewSpecialities");
	}
	m_pTreeView->set_scheme(DB::g_ModelSpecialities);
	m_pTreeView->append_column(_("id"), DB::g_ModelSpecialities.fId);
	m_pTreeView->append_column_editable(_("name"), DB::g_ModelSpecialities.name);
	m_pTreeView->append_column_editable(_("abbreviation"), DB::g_ModelSpecialities.abbr);
	m_pTreeView->append_column_foreign_editable(_("chair"), DB::g_ModelSpecialities.chair, DB::g_ModelChairs, DB::g_ModelChairs.abbr);
	m_pTreeView->signal_focus_in_event().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::OnFocusIn), m_pTreeView));
	m_pTreeView->signal_focus_out_event().connect(sigc::mem_fun(*this, &MainWindow::OnFocusOut));

#ifdef WIN32
	OnNew();
#endif

	show_all_children();
}

MainWindow::~MainWindow()
{
}

void MainWindow::OnNew()
{
	DB::DataBase::Instance().New();
}

void MainWindow::OnOpen()
{
	Gtk::FileChooserDialog dialog(*this,_("Choose file for opening database:"),
		Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialog.add_button(Gtk::Stock::CANCEL,Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::OPEN,Gtk::RESPONSE_YES);
	if(dialog.run()==Gtk::RESPONSE_YES)
	{
		DB::DataBase::Instance().Open(dialog.get_filename());
	}
}

void MainWindow::OnSave()
{
	Gtk::FileChooserDialog dialog(*this,_("Choose file for saving database:"),
	Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialog.add_button(Gtk::Stock::CANCEL,Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::SAVE,Gtk::RESPONSE_YES);
	if(dialog.run()==Gtk::RESPONSE_YES)
	{
		DB::DataBase::Instance().Save(dialog.get_filename());
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
}

void MainWindow::OnEdit()
{
	show_all_children();
}

void MainWindow::OnAppend()
{
	if(m_pCurrentListView)
	{
		m_pCurrentListView->add_empty_line();
		show_all_children();
	}
}

void MainWindow::OnDelete()
{
	if(m_pCurrentListView)
	{
		m_pCurrentListView->remove_line();
	}
}

bool MainWindow::OnFocusIn(GdkEventFocus* event, ListView *list_view)
{
	m_pCurrentListView = list_view;
	m_pCurrentListView->update_model();
	return false;
}

bool MainWindow::OnFocusOut(GdkEventFocus* event)
{
	m_pCurrentListView = 0;
	return false;
}

