#include <gtkmm/stock.h>
#include <gtkmm/filechooserdialog.h>
#include <iostream>
#include "main_window.h"
#include "shared.h"
#include "ga/ga.h"
#include "orm/data.h"
#include "orm/expr.h"
#include "db/models.h"

MainWindow::MainWindow(GtkWindow *cobject, const Glib::RefPtr<Gtk::Builder>& builder)
	:Gtk::Window(cobject),
	m_refBuilder(builder),
	m_HolydaysCategory(NULL),
	m_HolydaysObjectList(NULL),
	m_SheetHolydays(NULL),
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

	// connect Lists
	
	ListView* m_pTreeView = AddListView("TreeViewHours", DB::g_ModelHours);
	m_pTreeView->append_column_editable(_("start"), DB::g_ModelHours.start);
	m_pTreeView->append_column_editable(_("finish"), DB::g_ModelHours.finish);

	m_pTreeView = AddListView("TreeViewDays", DB::g_ModelDays);
	m_pTreeView->append_column_editable(_("name"), DB::g_ModelDays.name);

	m_pTreeView = AddListView("TreeViewFaculty", DB::g_ModelFaculties);
	m_pTreeView->append_column_editable(_("name"), DB::g_ModelFaculties.name);
	m_pTreeView->append_column_editable(_("abbreviation"), DB::g_ModelFaculties.abbr);

	m_pTreeView = AddListView("TreeViewChairs", DB::g_ModelChairs);
	m_pTreeView->append_column_editable(_("name"), DB::g_ModelChairs.name);
	m_pTreeView->append_column_editable(_("abbreviation"), DB::g_ModelChairs.abbr);
	m_pTreeView->append_column_foreign_editable(_("faculty"), DB::g_ModelChairs.faculty, DB::g_ModelFaculties, DB::g_ModelFaculties.abbr);

	m_pTreeView = AddListView("TreeViewTeachers", DB::g_ModelTeachers);
	m_pTreeView->append_column_editable(_("secondname"), DB::g_ModelTeachers.secondname);
	m_pTreeView->append_column_editable(_("firstname"), DB::g_ModelTeachers.firstname);
	m_pTreeView->append_column_editable(_("thirdname"), DB::g_ModelTeachers.thirdname);
	m_pTreeView->append_column_foreign_editable(_("chair"), DB::g_ModelTeachers.chair, DB::g_ModelChairs, DB::g_ModelChairs.abbr);

	m_pTreeView = AddListView("TreeViewSpecialities", DB::g_ModelSpecialities);
	m_pTreeView->append_column_editable(_("name"), DB::g_ModelSpecialities.name);
	m_pTreeView->append_column_editable(_("abbreviation"), DB::g_ModelSpecialities.abbr);
	m_pTreeView->append_column_foreign_editable(_("chair"), DB::g_ModelSpecialities.chair, DB::g_ModelChairs, DB::g_ModelChairs.abbr);

	m_pTreeView = AddListView("TreeViewGroups", DB::g_ModelGroups);
	m_pTreeView->append_column_editable(_("name"), DB::g_ModelGroups.name);
	m_pTreeView->append_column_foreign_editable(_("speciality"), DB::g_ModelGroups.speciality, DB::g_ModelSpecialities, DB::g_ModelSpecialities.abbr);

	m_pTreeView = AddListView("TreeViewBuildings", DB::g_ModelBuildings);
	m_pTreeView->append_column_editable(_("name"), DB::g_ModelBuildings.name);

	m_pTreeView = AddListView("TreeViewAuditoriums", DB::g_ModelAuditoriums);
	m_pTreeView->append_column_editable(_("name"), DB::g_ModelAuditoriums.name);
	m_pTreeView->append_column_editable(_("capacity"), DB::g_ModelAuditoriums.capacity);
	m_pTreeView->append_column_editable(_("multithread"), DB::g_ModelAuditoriums.multithread);
	m_pTreeView->append_column_foreign_editable(_("building"), DB::g_ModelAuditoriums.building, DB::g_ModelBuildings, DB::g_ModelBuildings.name);

	// Loadings -> Holydays
	
	m_refBuilder->get_widget("HolydaysCategory", m_HolydaysCategory);
	if(! m_HolydaysCategory)
	{
		throw Glib::Error(1, 0, "Cann't load HolydaysCategory");
	}
	m_HolydaysCategory->signal_changed().connect(sigc::mem_fun(*this, &MainWindow::SwitchHolydayCategory));

	m_refBuilder->get_widget("HolydaysObjectList", m_HolydaysObjectList);
	if(! m_HolydaysObjectList)
	{
		throw Glib::Error(1, 0, "Cann't load HolydaysObjectList");
	}
	m_HolydaysObjectList->signal_changed().connect(sigc::mem_fun(*this, &MainWindow::SwitchHolydayObject));

	m_refBuilder->get_widget_derived("SheetHolydays", m_SheetHolydays);
	if(! m_SheetHolydays)
	{
		throw Glib::Error(1, 0, "Cann't load SheetHolydays");
	}
	m_SheetHolydays->signal_cell_data().connect(sigc::mem_fun(*this, &MainWindow::HolydaysCellData));
	m_SheetHolydays->signal_cell_button_release().connect(sigc::mem_fun(*this, &MainWindow::HolydaysButtonRelease));

	OnNew();

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

ListView* MainWindow::AddListView(const Glib::ustring& name, const ORM::Table& scheme)
{
	ListView *res = 0;
	m_refBuilder->get_widget_derived(name, res);
	if(! res)
	{
		throw Glib::Error(1, 0, "Cann't load " + name);
	}
	res->set_scheme(scheme);
	res->append_column(_("id"), scheme.fId);
	res->signal_focus_in_event().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::OnFocusIn), res));
	res->signal_focus_out_event().connect(sigc::mem_fun(*this, &MainWindow::OnFocusOut));
	return res;
}

void MainWindow::SwitchHolydayCategory()
{
	Glib::ustring choose = m_HolydaysCategory->get_active_text();
	Glib::RefPtr<ORM::Data> data = ORM::Data::create(m_ComboScheme);
	if(choose == _("Teachers"))
	{
		// fill HolydayObjectList by teachers
		DB::DataBase::Instance().ListEntitiesText(DB::g_ModelTeachers, ORM::Expr<Glib::ustring>(ORM::Expr<Glib::ustring>(DB::g_ModelTeachers.secondname) + " " + DB::g_ModelTeachers.firstname + " " + DB::g_ModelTeachers.thirdname), data);
		m_HolydaysObjectList->set_model(data);
		m_HolydaysObjectList->set_text_column(m_ComboScheme.fText);
	}
	else if(choose == _("Groups"))
	{
		// fill HolydayObjectList by groups
		DB::DataBase::Instance().ListEntitiesText(DB::g_ModelGroups, DB::g_ModelGroups.name, data);
		m_HolydaysObjectList->set_model(data);
		m_HolydaysObjectList->set_text_column(m_ComboScheme.fText);
	}
	else
	{
		// clean data
		m_HolydaysObjectList->unset_model();
	}
}

void MainWindow::SwitchHolydayObject()
{
	//long value = m_HolydaysObjectList->get_active()->get_value(m_ComboScheme.fId);
	Glib::RefPtr<ORM::Data> vert_data = ORM::Data::create(m_ComboScheme);
	Glib::RefPtr<ORM::Data> horz_data = ORM::Data::create(m_ComboScheme);
	DB::DataBase::Instance().ListEntitiesText(DB::g_ModelHours, ORM::Expr<Glib::ustring>(ORM::Expr<Glib::ustring>(DB::g_ModelHours.start) + "-" + DB::g_ModelHours.finish), vert_data);
	DB::DataBase::Instance().ListEntitiesText(DB::g_ModelDays, DB::g_ModelDays.name, horz_data);
	m_SheetHolydays->set_vert_model(vert_data);
	m_SheetHolydays->set_horz_model(horz_data);
	
	show_all_children();
}

void MainWindow::HolydaysCellData(Gtk::CellRenderer *cell, long int row, long int column)
{
	Gtk::TreeIter obj_iter = m_HolydaysObjectList->get_active();
	if(obj_iter)
	{
		Gtk::CellRendererText *renderer = reinterpret_cast<Gtk::CellRendererText*>(cell);
		Glib::ustring category = m_HolydaysCategory->get_active_text();
		long int value = obj_iter->get_value(m_ComboScheme.fId);
		if(category == _("Teachers"))
		{
			if(DB::DataBase::Instance().GetTeacherHolydays(value, column, row))
			{
				renderer->property_text() = "+";
			}
			else
			{
				renderer->property_text() = "";
			}
		}
		else if(category == _("Groups"))
		{
			if(DB::DataBase::Instance().GetGroupHolydays(value, column, row))
			{
				renderer->property_text() = "+";
			}
			else
			{
				renderer->property_text() = "";
			}
		}
	}
}

void MainWindow::HolydaysButtonRelease(long int row, long int column, GdkEventButton* event)
{
	Gtk::TreeIter obj_iter = m_HolydaysObjectList->get_active();
	if(obj_iter)
	{
		Glib::ustring category = m_HolydaysCategory->get_active_text();
		long int value = obj_iter->get_value(m_ComboScheme.fId);
		if(category == _("Teachers"))
		{
			DB::DataBase::Instance().SetTeacherHolydays(value, column, row, ! DB::DataBase::Instance().GetTeacherHolydays(value, column, row));
			m_SheetHolydays->get_bin_window()->invalidate(true);
		}
		else if(category == _("Groups"))
		{
			DB::DataBase::Instance().SetGroupHolydays(value, column, row, ! DB::DataBase::Instance().GetGroupHolydays(value, column, row));
			m_SheetHolydays->get_bin_window()->invalidate(true);
		}

	}	
}

