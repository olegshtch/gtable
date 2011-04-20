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
	m_PlanSheet(NULL),
	m_ComboBoxPlanSpeciality(NULL),
	m_ScheduleGroup(NULL),
	m_ComboBoxScheduleGroup(NULL),
	m_pCurrentLineEditor(NULL),
	m_StatusBar(NULL)
{
	m_refBuilder->get_widget("StatusBar", m_StatusBar);
	if(! m_StatusBar)
	{
		throw Glib::Error(1, 0, "Cann't load StatusBar");
	}

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

	m_refActionGroup->add(Glib::RefPtr<Gtk::Action>::cast_dynamic(m_refBuilder->get_object("ScheduleNew")),sigc::mem_fun(*this, &MainWindow::OnScheduleNew));
	m_refActionGroup->add(Glib::RefPtr<Gtk::Action>::cast_dynamic(m_refBuilder->get_object("ScheduleCopy")),sigc::mem_fun(*this, &MainWindow::OnScheduleCopy));
	m_refActionGroup->add(Glib::RefPtr<Gtk::Action>::cast_dynamic(m_refBuilder->get_object("ScheduleDelete")),sigc::mem_fun(*this, &MainWindow::OnScheduleDelete));

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
	m_pTreeView->append_column_foreign_editable(_("chair"), DB::g_ModelAuditoriums.chair, DB::g_ModelChairs, DB::g_ModelChairs.abbr);

	m_pTreeView = AddListView("TreeViewBranchCategory", DB::g_ModelBranchCategory);
	m_pTreeView->append_column_editable(_("name"), DB::g_ModelBranchCategory.name);
	m_pTreeView->get_selection()->set_mode(Gtk::SELECTION_SINGLE);

	m_pTreeView = AddListView("TreeViewBranch", DB::g_ModelBranch);
	m_pTreeView->append_column_editable(_("name"), DB::g_ModelBranch.name);
	m_pTreeView->append_column_foreign_editable(_("category"), DB::g_ModelBranch.category, DB::g_ModelBranchCategory, DB::g_ModelBranchCategory.name);

	m_pTreeView = AddListView("TreeViewLessonType", DB::g_ModelLessonType);
	m_pTreeView->append_column_editable(_("name"), DB::g_ModelLessonType.name);
	m_pTreeView->append_column_editable(_("abbreviation"), DB::g_ModelLessonType.abbr);

	// Days -> Weeks
	
	m_DoubleWeek = 0;
	m_refBuilder->get_widget("CheckBoxWeeks", m_DoubleWeek);
	if(! m_DoubleWeek)
	{
		throw Glib::Error(1, 0, "Cann't load CheckBoxWeeks");
	}
	m_DoubleWeek->signal_toggled().connect(sigc::mem_fun(*this, &MainWindow::WeekToggle));

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

	// Loadings -> Plan

	m_refBuilder->get_widget_derived("PlanSheet", m_PlanSheet);
	if(! m_PlanSheet)
	{
		throw Glib::Error(1, 0, "Cann't load PlanSheet");
	}
	m_PlanSheet->signal_focus_in_event().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::OnFocusIn), static_cast<LineEditable*>(m_PlanSheet)));
	m_PlanSheet->signal_focus_out_event().connect(sigc::mem_fun(*this, &MainWindow::OnFocusOut));
	m_PlanSheet->signal_cell_data().connect(sigc::mem_fun(*this, &MainWindow::PlanSpecialitiesCellData));
	m_PlanSheet->signal_cell_button_release().connect(sigc::mem_fun(*this, &MainWindow::PlanSpecialitiesButtonRelease));

	m_refBuilder->get_widget("ComboBoxPlanSpeciality", m_ComboBoxPlanSpeciality);
	if(! m_ComboBoxPlanSpeciality)
	{
		throw Glib::Error(1, 0, "Cann't load ComboBoxPlanSpeciality");
	}
	m_ComboBoxPlanSpeciality->signal_expose_event().connect(sigc::mem_fun(*this, &MainWindow::PlanSpecialitiesExpose));
	m_ComboBoxPlanSpeciality->set_model(ORM::Data::create(m_ComboScheme));
	m_ComboBoxPlanSpeciality->pack_start(m_ComboScheme.fText);
	m_ComboBoxPlanSpeciality->signal_changed().connect(sigc::mem_fun(*this, &MainWindow::PlanSpecialitiesChanged));

	// Schedule -> Group
	
	m_refBuilder->get_widget_derived("ScheduleGroup", m_ScheduleGroup);
	if(! m_ScheduleGroup)
	{
		throw Glib::Error(1, 0, "Cann't load ScheduleGroup");
	}
	m_refBuilder->get_widget("ComboBoxScheduleGroup", m_ComboBoxScheduleGroup);
	m_ScheduleGroup->signal_cell_data().connect(sigc::mem_fun(*this, &MainWindow::ScheduleGroupCellData));
	if(! m_ComboBoxScheduleGroup)
	{
		throw Glib::Error(1, 0, "Cann't load ComboBoxScheduleGroup");
	}
	m_ComboBoxScheduleGroup->signal_expose_event().connect(sigc::mem_fun(*this, &MainWindow::ScheduleGroupExpose));
	m_ComboBoxScheduleGroup->set_model(ORM::Data::create(m_ComboScheme));
	m_ComboBoxScheduleGroup->pack_start(m_ComboScheme.fText);
	m_ComboBoxScheduleGroup->signal_changed().connect(sigc::mem_fun(*this, &MainWindow::ScheduleGroupChanged));

	OnNew();

	m_DoubleWeek->property_active() = DB::DataBase::Instance().GetWeeks();

	show_all_children();

	Glib::add_exception_handler(sigc::mem_fun(*this, &MainWindow::OnException));
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
	m_DoubleWeek->property_active() = DB::DataBase::Instance().GetWeeks();

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
	if(m_pCurrentLineEditor)
	{
		m_pCurrentLineEditor->add_empty_line();
		show_all_children();
	}
}

void MainWindow::OnDelete()
{
	if(m_pCurrentLineEditor)
	{
		m_pCurrentLineEditor->remove_line();
	}
}

bool MainWindow::OnFocusIn(GdkEventFocus* event, LineEditable *line_editor)
{
	m_pCurrentLineEditor = line_editor;
	m_pCurrentLineEditor->update_model();
	return false;
}

bool MainWindow::OnFocusOut(GdkEventFocus* event)
{
	m_pCurrentLineEditor = 0;
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
	res->signal_focus_in_event().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::OnFocusIn), static_cast<LineEditable*>(res)));
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

void MainWindow::WeekToggle()
{
	DB::DataBase::Instance().SetWeeks(m_DoubleWeek->property_active());
}

bool MainWindow::PlanSpecialitiesExpose(GdkEventExpose* event)
{
	Glib::RefPtr<ORM::Data> data = ORM::Data::create(m_ComboScheme);
	DB::DataBase::Instance().ListEntitiesText(DB::g_ModelSpecialities, DB::g_ModelSpecialities.name, data);
	m_ComboBoxPlanSpeciality->set_model(data);
	return false;
}

void MainWindow::PlanSpecialitiesChanged()
{
	Gtk::TreeIter iter = m_ComboBoxPlanSpeciality->get_active();
	if(iter)
	{
		m_PlanSheet->set_speciality(iter->get_value(m_ComboScheme.fId));
		m_PlanSheet->update_model();
	}
}

void MainWindow::PlanSpecialitiesCellData(Gtk::CellRenderer *cell, long int id_teaching_branch, long int id_lesson_type)
{
	Gtk::CellRendererText *renderer = reinterpret_cast<Gtk::CellRendererText *>(cell);
	long value = DB::DataBase::Instance().GetTeachingPlanHours(id_teaching_branch, id_lesson_type);
	if(value > 0)
	{
		renderer->property_text() = ORM::Field<long>::ToString(value);
	}
	else
	{
		renderer->property_text() = "";
	}
}

void MainWindow::PlanSpecialitiesButtonRelease(long int id_teaching_branch, long int id_lesson_type, GdkEventButton* event)
{
	long value = DB::DataBase::Instance().GetTeachingPlanHours(id_teaching_branch, id_lesson_type);
	switch(event->button)
	{
		case 1:
			DB::DataBase::Instance().EditTeachingPlanHours(id_teaching_branch, id_lesson_type, value + 1);
			break;
		case 3:
			DB::DataBase::Instance().EditTeachingPlanHours(id_teaching_branch, id_lesson_type, (value > 0) ? (value - 1) : 0);
			break;
	}
	m_PlanSheet->get_bin_window()->invalidate(true);
}

bool MainWindow::ScheduleGroupExpose(GdkEventExpose* event)
{
	Glib::RefPtr<ORM::Data> data = ORM::Data::create(m_ComboScheme);
	DB::DataBase::Instance().ListEntitiesText(DB::g_ModelGroups, DB::g_ModelGroups.name, data);
	m_ComboBoxScheduleGroup->set_model(data);
	return false;
}

void MainWindow::ScheduleGroupChanged()
{
	Gtk::TreeIter iter = m_ComboBoxScheduleGroup->get_active();
	if(iter)
	{
		Glib::RefPtr<ORM::Data> vert_data = ORM::Data::create(m_ComboScheme);
		Glib::RefPtr<ORM::Data> horz_data = ORM::Data::create(m_ComboScheme);
		DB::DataBase::Instance().ListEntitiesText(DB::g_ModelHours, ORM::Expr<Glib::ustring>(ORM::Expr<Glib::ustring>(DB::g_ModelHours.start) + "-" + DB::g_ModelHours.finish), vert_data);
		DB::DataBase::Instance().ListEntitiesText(DB::g_ModelDays, DB::g_ModelDays.name, horz_data);
		m_ScheduleGroup->set_vert_model(vert_data);
		m_ScheduleGroup->set_horz_model(horz_data);
	}
}

// yet fake!!!
void MainWindow::ScheduleGroupCellData(Gtk::CellRenderer* cell, long int id_hour, long int id_day)
{
	Gtk::CellRendererText *renderer = reinterpret_cast<Gtk::CellRendererText *>(cell);
	if((id_hour >= 4) && (id_hour <= 6) && (id_day >= 1) && (id_day <= 5))
	{
		renderer->property_markup() = Glib::ustring::compose("<b>%1</b><i>%2</i>", id_hour, id_day);
		renderer->set_fixed_size(150, 100);
	}
	else
	{
		renderer->property_markup() = "";
		renderer->set_fixed_size(-1, -1);
	}
}

void MainWindow::OnScheduleNew()
{
}

void MainWindow::OnScheduleCopy()
{
}

void MainWindow::OnScheduleDelete()
{
}

void MainWindow::OnException()
{
	try
	{
		throw;
	}
	catch(std::exception& e)
	{
		m_StatusBar->remove_all_messages();
		m_StatusBar->push(e.what());
	}
	catch(Glib::Exception& e)
	{
		m_StatusBar->remove_all_messages();
		m_StatusBar->push(e.what());
	}
}

