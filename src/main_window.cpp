#include <gtkmm/stock.h>
#include <gtkmm/filechooserdialog.h>
#include <iostream>
#include "main_window.h"
#include "shared.h"
#include "ga/graph.h"
#include "orm/data.h"
#include "orm/expr.h"
#include "db/models.h"
#include "logbuf.h"

MainWindow::MainWindow(GtkWindow *cobject, const Glib::RefPtr<Gtk::Builder>& builder)
	:Gtk::Window(cobject),
	m_refBuilder(builder),
	m_HolydaysCategory(NULL),
	m_HolydaysObjectList(NULL),
	m_HolydaysCategoryList(ORM::Data::create(DB::g_IdTextScheme)),
	m_SheetHolydays(NULL),
	m_PlanSheet(NULL),
	m_ComboBoxPlanSpeciality(NULL),

	m_ScheduleGroup(NULL),
	m_ComboBoxScheduleGroup(NULL),
	m_ScheduleGroupOther(NULL),
	m_ScheduleIdGroup(-1),

	m_ScheduleTeacher(NULL),
	m_ComboBoxScheduleTeacher(NULL),
	m_ScheduleTeacherOther(NULL),
	m_ScheduleIdTeacher(-1),

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
	m_refActionGroup->add(Glib::RefPtr<Gtk::Action>::cast_dynamic(m_refBuilder->get_object("ScheduleRun")),sigc::mem_fun(*this, &MainWindow::OnScheduleRun));

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
	m_pTreeView->signal_list_edited().connect(sigc::mem_fun(*this, &MainWindow::ScheduleTeacherExpose));

	m_pTreeView = AddListView("TreeViewSpecialities", DB::g_ModelSpecialities);
	m_pTreeView->append_column_editable(_("name"), DB::g_ModelSpecialities.name);
	m_pTreeView->append_column_editable(_("abbreviation"), DB::g_ModelSpecialities.abbr);
	m_pTreeView->append_column_foreign_editable(_("chair"), DB::g_ModelSpecialities.chair, DB::g_ModelChairs, DB::g_ModelChairs.abbr);
	m_pTreeView->append_column_editable(_("terms"), DB::g_ModelSpecialities.terms);
	m_pTreeView->signal_list_edited().connect(sigc::mem_fun(*this, &MainWindow::PlanSpecialitiesExpose));

	m_pTreeView = AddListView("TreeViewGroups", DB::g_ModelGroups);
	m_pTreeView->append_column_editable(_("name"), DB::g_ModelGroups.name);
	m_pTreeView->append_column_foreign_editable(_("speciality"), DB::g_ModelGroups.speciality, DB::g_ModelSpecialities, DB::g_ModelSpecialities.abbr);
	m_pTreeView->append_column_editable(_("term"), DB::g_ModelGroups.term);
	m_pTreeView->signal_list_edited().connect(sigc::mem_fun(*this, &MainWindow::TeachingLessonGroupExpose));
	m_pTreeView->signal_list_edited().connect(sigc::mem_fun(*this, &MainWindow::ScheduleGroupExpose));

	m_pTreeView = AddListView("TreeViewBuildings", DB::g_ModelBuildings);
	m_pTreeView->append_column_editable(_("name"), DB::g_ModelBuildings.name);

	m_pTreeView = AddListView("TreeViewAuditoriums", DB::g_ModelAuditoriums);
	m_pTreeView->append_column_editable(_("name"), DB::g_ModelAuditoriums.name);
	m_pTreeView->append_column_editable(_("capacity"), DB::g_ModelAuditoriums.capacity);
	m_pTreeView->append_column_editable(_("multithread"), DB::g_ModelAuditoriums.multithread);
	m_pTreeView->append_column_foreign_editable(_("building"), DB::g_ModelAuditoriums.building, DB::g_ModelBuildings, DB::g_ModelBuildings.name);
	m_pTreeView->append_column_foreign_editable(_("chair"), DB::g_ModelAuditoriums.chair, DB::g_ModelChairs, DB::g_ModelChairs.abbr);

	m_pTreeView = AddListView("TreeViewAuditoriumTypes", DB::g_ModelAuditoriumTypes);
	m_pTreeView->append_column_editable(_("property"), DB::g_ModelAuditoriumTypes.name);

	m_pTreeView = AddListView("TreeViewBranchCategory", DB::g_ModelBranchCategory);
	m_pTreeView->append_column_editable(_("name"), DB::g_ModelBranchCategory.name);
	m_pTreeView->get_selection()->set_mode(Gtk::SELECTION_SINGLE);

	m_pTreeView = AddListView("TreeViewBranch", DB::g_ModelBranch);
	m_pTreeView->append_column_editable(_("name"), DB::g_ModelBranch.name);
	m_pTreeView->append_column_editable(_("abbreviation"), DB::g_ModelBranch.abbr);
	m_pTreeView->append_column_foreign_editable(_("category"), DB::g_ModelBranch.category, DB::g_ModelBranchCategory, DB::g_ModelBranchCategory.name);

	m_pTreeView = AddListView("TreeViewLessonType", DB::g_ModelLessonType);
	m_pTreeView->append_column_editable(_("name"), DB::g_ModelLessonType.name);
	m_pTreeView->append_column_editable(_("abbreviation"), DB::g_ModelLessonType.abbr);
	m_pTreeView->append_column_foreign_editable(_("before"), DB::g_ModelLessonType.before, DB::g_ModelLessonType, DB::g_ModelLessonType.abbr);
	m_pTreeView->append_column_editable(_("multithread"), DB::g_ModelLessonType.multithread);

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
	Gtk::TreeIter iter = m_HolydaysCategoryList->append();
	iter->set_value(DB::g_IdTextScheme.fId, 1l);
	iter->set_value(DB::g_IdTextScheme.fText, Glib::ustring(_("Teachers")));
	iter = m_HolydaysCategoryList->append();
	iter->set_value(DB::g_IdTextScheme.fId, 2l);
	iter->set_value(DB::g_IdTextScheme.fText, Glib::ustring(_("Groups")));
	iter = m_HolydaysCategoryList->append();
	iter->set_value(DB::g_IdTextScheme.fId, 3l);
	iter->set_value(DB::g_IdTextScheme.fText, Glib::ustring(_("Auditoriums")));
	m_HolydaysIdCategory = -1;
	m_HolydaysCategory->set_model(m_HolydaysCategoryList);
	m_HolydaysCategory->pack_start(DB::g_IdTextScheme.fText);
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
	m_LineEditors.push_back(m_PlanSheet);
	m_PlanSheet->signal_focus_in_event().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::OnFocusIn), static_cast<LineEditable*>(m_PlanSheet)));
	m_PlanSheet->signal_focus_out_event().connect(sigc::mem_fun(*this, &MainWindow::OnFocusOut));
	m_PlanSheet->signal_cell_data().connect(sigc::mem_fun(*this, &MainWindow::PlanSpecialitiesCellData));
	m_PlanSheet->signal_cell_button_release().connect(sigc::mem_fun(*this, &MainWindow::PlanSpecialitiesButtonRelease));

	m_refBuilder->get_widget("ComboBoxPlanSpeciality", m_ComboBoxPlanSpeciality);
	if(! m_ComboBoxPlanSpeciality)
	{
		throw Glib::Error(1, 0, "Cann't load ComboBoxPlanSpeciality");
	}
	m_ComboBoxPlanSpeciality->set_model(ORM::Data::create(DB::g_IdTextScheme));
	m_ComboBoxPlanSpeciality->pack_start(DB::g_IdTextScheme.fText);
	m_ComboBoxPlanSpeciality->signal_changed().connect(sigc::mem_fun(*this, &MainWindow::PlanSpecialitiesChanged));

	// Loadings -> Teaching lesson
	m_refBuilder->get_widget_derived("TreeViewTeachedLesson", m_TeachingLesson);
	if(! m_TeachingLesson)
	{
		throw Glib::Error(1, 0, "Cann't load TreeViewTeachedLesson");
	}

	m_refBuilder->get_widget("TeachedLessonGroup", m_ComboBoxTeachingLesson);
	if(! m_ComboBoxTeachingLesson)
	{
		throw Glib::Error(1, 0, "Cann't load TeachedLessonGroup");
	}
	m_ComboBoxTeachingLesson->set_model(ORM::Data::create(DB::g_IdTextScheme));
	m_ComboBoxTeachingLesson->pack_start(DB::g_IdTextScheme.fText);
	m_ComboBoxTeachingLesson->signal_changed().connect(sigc::mem_fun(*this, &MainWindow::TeachingLessonGroupChanged));

	// Loadings -> Auditorium Lessons
	
	m_pTreeView = AddListView("TreeViewAllAuditoriumTypes", DB::g_ModelAuditoriumTypes);
	m_pTreeView->append_column_editable(_("property"), DB::g_ModelAuditoriumTypes.name);

	// Schedule -> Group
	
	m_refBuilder->get_widget_derived("ScheduleGroup", m_ScheduleGroup);
	if(! m_ScheduleGroup)
	{
		throw Glib::Error(1, 0, "Cann't load ScheduleGroup");
	}
	m_ScheduleGroup->signal_cell_data().connect(sigc::mem_fun(*this, &MainWindow::ScheduleGroupCellData));
	m_ScheduleGroup->signal_cell_button_release().connect(sigc::mem_fun(*this, &MainWindow::ScheduleGroupCellButtonRelease));
	m_refBuilder->get_widget("ComboBoxScheduleGroup", m_ComboBoxScheduleGroup);
	if(! m_ComboBoxScheduleGroup)
	{
		throw Glib::Error(1, 0, "Cann't load ComboBoxScheduleGroup");
	}
	m_ComboBoxScheduleGroup->set_model(ORM::Data::create(DB::g_IdTextScheme));
	m_ComboBoxScheduleGroup->pack_start(DB::g_IdTextScheme.fText);
	m_ComboBoxScheduleGroup->signal_changed().connect(sigc::mem_fun(*this, &MainWindow::ScheduleGroupChanged));

	m_refBuilder->get_widget("TreeViewGroupLessonsOther", m_ScheduleGroupOther);
	if(! m_ScheduleGroupOther)
	{
		throw Glib::Error(1, 0, "Cann't load ComboBoxScheduleGroup");
	}
	m_ScheduleGroupOther->set_model(ORM::Data::create(DB::g_IdTextScheme));
	m_ScheduleGroupOther->set_headers_visible(false);
	m_ScheduleGroupOther->append_column(_(""), DB::g_IdTextScheme.fText);
	m_ScheduleGroupOther->get_selection()->set_mode(Gtk::SELECTION_SINGLE);
	m_ScheduleGroupOther->get_selection()->signal_changed().connect(sigc::mem_fun(*this, &MainWindow::ScheduleGroupSelectedOther));

	// Schedule -> Teacher
	m_refBuilder->get_widget_derived("ScheduleTeacher", m_ScheduleTeacher);
	if(! m_ScheduleTeacher)
	{
		throw Glib::Error(1, 0, "Cann't load ScheduleTeacher");
	}
	m_ScheduleTeacher->signal_cell_data().connect(sigc::mem_fun(*this, &MainWindow::ScheduleTeacherCellData));
	m_ScheduleTeacher->signal_cell_button_release().connect(sigc::mem_fun(*this, &MainWindow::ScheduleTeacherCellButtonRelease));
	m_refBuilder->get_widget("ComboBoxScheduleTeacher", m_ComboBoxScheduleTeacher);
	if(! m_ComboBoxScheduleTeacher)
	{
		throw Glib::Error(1, 0, "ComboBoxScheduleTeacher");
	}
	m_ComboBoxScheduleTeacher->set_model(ORM::Data::create(DB::g_IdTextScheme));
	m_ComboBoxScheduleTeacher->pack_start(DB::g_IdTextScheme.fText);
	m_ComboBoxScheduleTeacher->signal_changed().connect(sigc::mem_fun(*this, &MainWindow::ScheduleTeacherChanged));
	m_refBuilder->get_widget("TreeViewTeacherLessonOther", m_ScheduleTeacherOther);
	if(! m_ScheduleTeacherOther)
	{
		throw Glib::Error(1, 0, "Cann't load TreeViewTeacherLessonOther");
	}
	m_ScheduleTeacherOther->set_model(ORM::Data::create(DB::g_IdTextScheme));
	m_ScheduleTeacherOther->set_headers_visible(false);
	m_ScheduleTeacherOther->append_column(_(""), DB::g_IdTextScheme.fText);
	m_ScheduleTeacherOther->get_selection()->set_mode(Gtk::SELECTION_SINGLE);
	m_ScheduleTeacherOther->get_selection()->signal_changed().connect(sigc::mem_fun(*this, &MainWindow::ScheduleTeacherSelectedOther));

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
	for(std::vector<LineEditable*>::iterator it = m_LineEditors.begin(); it != m_LineEditors.end(); ++ it)
	{
		(*it)->update_model();
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
	m_LineEditors.push_back(res);
	res->signal_focus_in_event().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::OnFocusIn), static_cast<LineEditable*>(res)));
	res->signal_focus_out_event().connect(sigc::mem_fun(*this, &MainWindow::OnFocusOut));
	return res;
}

void MainWindow::SwitchHolydayCategory()
{
	std::clog << "MainWindow::SwitchHolydayCategory" << std::endl;
	Glib::RefPtr<ORM::Data> data = ORM::Data::create(DB::g_IdTextScheme);
	Gtk::TreeIter iter = m_HolydaysCategory->get_active();
	if(iter)
	{
		m_HolydaysIdCategory = iter->get_value(DB::g_IdTextScheme.fId);
	}
	switch(m_HolydaysIdCategory)
	{
	case 1:
		// fill HolydayObjectList by teachers
		DB::DataBase::Instance().ListEntitiesText(DB::g_ModelTeachers, ORM::Expr<Glib::ustring>(DB::g_ModelTeachers.secondname) + " " + DB::g_ModelTeachers.firstname + " " + DB::g_ModelTeachers.thirdname, data);
		m_HolydaysObjectList->set_model(data);
		m_HolydaysObjectList->set_text_column(DB::g_IdTextScheme.fText);
		break;
	case 2:
		// fill HolydayObjectList by groups
		DB::DataBase::Instance().ListEntitiesText(DB::g_ModelGroups, DB::g_ModelGroups.name, data);
		m_HolydaysObjectList->set_model(data);
		m_HolydaysObjectList->set_text_column(DB::g_IdTextScheme.fText);
		break;
	case 3:
		DB::DataBase::Instance().ListEntitiesText(DB::g_ModelAuditoriums, DB::g_ModelAuditoriums.name, data);
		m_HolydaysObjectList->set_model(data);
		m_HolydaysObjectList->set_text_column(DB::g_IdTextScheme.fText);
		break;
	default:
		// clean data
		m_HolydaysObjectList->unset_model();
	}
}

void MainWindow::SwitchHolydayObject()
{
	//long value = m_HolydaysObjectList->get_active()->get_value(m_ComboScheme.fId);
	Glib::RefPtr<ORM::Data> vert_data = ORM::Data::create(DB::g_IdTextScheme);
	Glib::RefPtr<ORM::Data> horz_data = ORM::Data::create(DB::g_IdTextScheme);
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
		long int value = obj_iter->get_value(DB::g_IdTextScheme.fId);
		switch(m_HolydaysIdCategory)
		{
		case 1:
			if(DB::DataBase::Instance().GetTeacherHolydays(value, column, row))
			{
				renderer->property_text() = "+";
			}
			else
			{
				renderer->property_text() = "";
			}
			break;
		case 2:
			if(DB::DataBase::Instance().GetGroupHolydays(value, column, row))
			{
				renderer->property_text() = "+";
			}
			else
			{
				renderer->property_text() = "";
			}
			break;
		case 3:
			if(DB::DataBase::Instance().GetAuditoriumHolydays(value, column, row))
			{
				renderer->property_text() = "+";
			}
			else
			{
				renderer->property_text() = "";
			}
			break;
		}
	}
}

void MainWindow::HolydaysButtonRelease(long int row, long int column, GdkEventButton* event)
{
	Gtk::TreeIter obj_iter = m_HolydaysObjectList->get_active();
	if(obj_iter)
	{
		long int value = obj_iter->get_value(DB::g_IdTextScheme.fId);
		switch(m_HolydaysIdCategory)
		{
		case 1:
			DB::DataBase::Instance().SetTeacherHolydays(value, column, row, ! DB::DataBase::Instance().GetTeacherHolydays(value, column, row));
			break;
		case 2:
			DB::DataBase::Instance().SetGroupHolydays(value, column, row, ! DB::DataBase::Instance().GetGroupHolydays(value, column, row));
			break;
		case 3:
			DB::DataBase::Instance().SetAuditoriumHolydays(value, column, row, ! DB::DataBase::Instance().GetGroupHolydays(value, column, row));
			break;
		}
		m_SheetHolydays->get_bin_window()->invalidate(true);
	}	
}

void MainWindow::WeekToggle()
{
	DB::DataBase::Instance().SetWeeks(m_DoubleWeek->property_active());
}

void MainWindow::PlanSpecialitiesExpose()
{
	Glib::RefPtr<ORM::Data> data = ORM::Data::create(DB::g_IdTextScheme);
	DB::DataBase::Instance().ListEntitiesText(DB::g_ModelSpecialities, DB::g_ModelSpecialities.name, data);
	m_ComboBoxPlanSpeciality->set_model(data);
}

void MainWindow::PlanSpecialitiesChanged()
{
	Gtk::TreeIter iter = m_ComboBoxPlanSpeciality->get_active();
	if(iter)
	{
		m_PlanSheet->set_speciality(iter->get_value(DB::g_IdTextScheme.fId));
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
		case 1: //left button
			DB::DataBase::Instance().EditTeachingPlanHours(id_teaching_branch, id_lesson_type, value + 1);
			break;
		case 3: //right button
			DB::DataBase::Instance().EditTeachingPlanHours(id_teaching_branch, id_lesson_type, (value > 0) ? (value - 1) : 0);
			break;
	}
	m_PlanSheet->get_bin_window()->invalidate(true);
}

void MainWindow::ScheduleGroupExpose()
{
	Glib::RefPtr<ORM::Data> data = ORM::Data::create(DB::g_IdTextScheme);
	DB::DataBase::Instance().ListEntitiesText(DB::g_ModelGroups, DB::g_ModelGroups.name, data);
	m_ComboBoxScheduleGroup->set_model(data);
}

void MainWindow::ScheduleGroupChanged()
{
	Gtk::TreeIter iter = m_ComboBoxScheduleGroup->get_active();
	if(iter)
	{
		m_ScheduleIdGroup = iter->get_value(DB::g_IdTextScheme.fId);
	}
	if(m_ScheduleIdGroup != -1)
	{
		Glib::RefPtr<ORM::Data> vert_data = ORM::Data::create(DB::g_IdTextScheme);
		Glib::RefPtr<ORM::Data> horz_data = ORM::Data::create(DB::g_IdTextScheme);
		DB::DataBase::Instance().ListEntitiesText(DB::g_ModelHours, ORM::Expr<Glib::ustring>(ORM::Expr<Glib::ustring>(DB::g_ModelHours.start) + "-" + DB::g_ModelHours.finish), vert_data);
		DB::DataBase::Instance().ListEntitiesText(DB::g_ModelDays, DB::g_ModelDays.name, horz_data);
		m_ScheduleGroup->set_vert_model(vert_data);
		m_ScheduleGroup->set_horz_model(horz_data);

		Glib::RefPtr<ORM::Data> other_data = ORM::Data::create(DB::g_IdTextScheme);
		DB::DataBase::Instance().ListGroupOtherLessons(m_ScheduleIdGroup, other_data);
		m_ScheduleGroupOther->set_model(other_data);
	}
}

void MainWindow::ScheduleGroupCellData(Gtk::CellRenderer* cell, long int id_hour, long int id_day)
{
	Gtk::CellRendererText *renderer = reinterpret_cast<Gtk::CellRendererText *>(cell);
	renderer->property_text() = DB::DataBase::Instance().GetTimeTableLessonGroupText(m_ScheduleIdGroup, id_hour, id_day);
	if(DB::DataBase::Instance().GetGroupHolydays(m_ScheduleIdGroup, id_day, id_hour))
	{
		renderer->property_background_gdk() = Gdk::Color("red");
	}
	else
	{
		renderer->property_background_gdk() = Gdk::Color("white");
	}
}

void MainWindow::ScheduleGroupSelectedOther()
{
	m_ScheduleGroupSelectedOther = m_ScheduleGroupOther->get_selection()->get_selected();
}

void MainWindow::ScheduleGroupCellButtonRelease(long int id_hour, long int id_day, GdkEventButton* event)
{
	switch(event->button)
	{
	case 1: // left button
		if(m_ScheduleGroupSelectedOther)
		{
			long int lesson_id = m_ScheduleGroupSelectedOther->get_value(DB::g_IdTextScheme.fId);
			//move lesson to timetable
			//get auditoriums list
			Glib::RefPtr<ORM::Data> aud_list = ORM::Data::create(DB::g_IdTextScheme);
			DB::DataBase::Instance().GetAuditoriumListForLesson(aud_list, lesson_id, id_day, id_hour);
			if(aud_list->children().size() > 0)
			{
				m_ScheduleIdDay = id_day;
				m_ScheduleIdHour = id_hour;
				m_ScheduleMenu.items().erase(m_ScheduleMenu.items().begin(), m_ScheduleMenu.items().end());
				for(Gtk::TreeIter it = aud_list->children().begin(); it != aud_list->children().end(); ++ it)
				{
					Gtk::MenuItem *p_item = Gtk::manage(new Gtk::MenuItem(it->get_value(DB::g_IdTextScheme.fText)));
					m_ScheduleMenu.append(*p_item);
					p_item->signal_activate().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::ScheduleGroupChooseAud), it->get_value(DB::g_IdTextScheme.fId)));
					p_item->show();
				}
				m_ScheduleMenu.popup(0, event->time);
				//std::cout << "popup size: " << m_ScheduleMenu.items().size() << std::endl;
			}
			else
			{
				std::cout << "Cann't found auditoriums" << std::endl;
			}
		}
		break;
	case 3: // change auditorium or delete
		{
			long int lesson_id = DB::DataBase::Instance().GetTimeTableLessonGroup(m_ScheduleIdGroup, id_hour, id_day);
			if(lesson_id != -1)
			{
				//lesson exist
				m_ScheduleIdDay = id_day;
				m_ScheduleIdHour = id_hour;
				Glib::RefPtr<ORM::Data> aud_list = ORM::Data::create(DB::g_IdTextScheme);
				DB::DataBase::Instance().GetAuditoriumListForLesson(aud_list, lesson_id, id_day, id_hour);
				m_ScheduleMenu.items().erase(m_ScheduleMenu.items().begin(), m_ScheduleMenu.items().end());
				if(aud_list->children().size() > 0)
				{
					for(Gtk::TreeIter it = aud_list->children().begin(); it != aud_list->children().end(); ++ it)
					{
						Gtk::MenuItem *p_item = Gtk::manage(new Gtk::MenuItem(it->get_value(DB::g_IdTextScheme.fText)));
						m_ScheduleMenu.append(*p_item);
						p_item->signal_activate().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::ScheduleGroupChangeAud), lesson_id, it->get_value(DB::g_IdTextScheme.fId)));
						//p_item->show();
					}
					m_ScheduleMenu.append(*Gtk::manage(new Gtk::SeparatorMenuItem()));
					//std::cout << "popup size: " << m_ScheduleMenu.items().size() << std::endl;
				}
				else
				{
					std::cout << "Cann't found auditoriums" << std::endl;
				}
				// add remove item
				Gtk::MenuItem *p_item = Gtk::manage(new Gtk::MenuItem(_("Delete")));
				m_ScheduleMenu.append(*p_item);
				p_item->signal_activate().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::ScheduleGroupRemoveLesson), lesson_id));
				//p_item->show();
				m_ScheduleMenu.show_all_children();
				m_ScheduleMenu.popup(0, event->time);
			}
		}
		break;
	}
}

void MainWindow::ScheduleGroupChooseAud(long int aud_id)
{
	std::cout << "Choose " << aud_id << " aud" << std::endl;
	if(m_ScheduleGroupSelectedOther)
	{
		long int lesson_id = m_ScheduleGroupSelectedOther->get_value(DB::g_IdTextScheme.fId);
		DB::DataBase::Instance().SetLessonIntoTimetable(lesson_id, aud_id, m_ScheduleIdDay, m_ScheduleIdHour);
		/*m_TeachingLesson->resize_children();
		m_TeachingLesson->columns_autosize();
		//m_TeachingLesson->get_bin_window()->invalidate(true);

		Glib::RefPtr<ORM::Data> other_data = ORM::Data::create(DB::g_IdTextScheme);
		DB::DataBase::Instance().ListGroupOtherLessons(m_ScheduleIdGroup, other_data);
		m_ScheduleGroupOther->set_model(other_data);

		m_ScheduleGroupSelectedOther = Gtk::TreeIter();*/
		ScheduleGroupChanged();
	}
}

void MainWindow::ScheduleGroupChangeAud(long int lesson_id, long int aud_id)
{
	DB::DataBase::Instance().RemoveLessonFromTimetable(lesson_id, m_ScheduleIdDay, m_ScheduleIdHour);
	DB::DataBase::Instance().SetLessonIntoTimetable(lesson_id, aud_id, m_ScheduleIdDay, m_ScheduleIdHour);
	/*
	Glib::RefPtr<ORM::Data> other_data = ORM::Data::create(DB::g_IdTextScheme);
	DB::DataBase::Instance().ListGroupOtherLessons(m_ScheduleIdGroup, other_data);
	m_ScheduleGroupOther->set_model(other_data);
	m_ScheduleGroupSelectedOther = Gtk::TreeIter();*/
	ScheduleGroupChanged();
}

void MainWindow::ScheduleGroupRemoveLesson(long int lesson_id)
{
	DB::DataBase::Instance().RemoveLessonFromTimetable(lesson_id, m_ScheduleIdDay, m_ScheduleIdHour);
	/*
	Glib::RefPtr<ORM::Data> other_data = ORM::Data::create(DB::g_IdTextScheme);
	DB::DataBase::Instance().ListGroupOtherLessons(m_ScheduleIdGroup, other_data);
	m_ScheduleGroupOther->set_model(other_data);
	m_ScheduleGroupSelectedOther = Gtk::TreeIter();*/
	ScheduleGroupChanged();
}

void MainWindow::ScheduleTeacherExpose()
{
	Glib::RefPtr<ORM::Data> data = ORM::Data::create(DB::g_IdTextScheme);
	DB::DataBase::Instance().ListEntitiesText(DB::g_ModelTeachers, ORM::Expr<Glib::ustring>(DB::g_ModelTeachers.secondname) + " " + DB::g_ModelTeachers.firstname + " " + DB::g_ModelTeachers.thirdname, data);
	m_ComboBoxScheduleTeacher->set_model(data);
}

void MainWindow::ScheduleTeacherChanged()
{
	Gtk::TreeIter iter = m_ComboBoxScheduleTeacher->get_active();
	if(iter)
	{
		m_ScheduleIdTeacher = iter->get_value(DB::g_IdTextScheme.fId);
	}
	if(m_ScheduleIdTeacher != -1)
	{
		Glib::RefPtr<ORM::Data> vert_data = ORM::Data::create(DB::g_IdTextScheme);
		Glib::RefPtr<ORM::Data> horz_data = ORM::Data::create(DB::g_IdTextScheme);
		DB::DataBase::Instance().ListEntitiesText(DB::g_ModelHours, ORM::Expr<Glib::ustring>(ORM::Expr<Glib::ustring>(DB::g_ModelHours.start) + "-" + DB::g_ModelHours.finish), vert_data);
		DB::DataBase::Instance().ListEntitiesText(DB::g_ModelDays, DB::g_ModelDays.name, horz_data);
		m_ScheduleTeacher->set_vert_model(vert_data);
		m_ScheduleTeacher->set_horz_model(horz_data);

		Glib::RefPtr<ORM::Data> other_data = ORM::Data::create(DB::g_IdTextScheme);
		DB::DataBase::Instance().ListTeacherOtherLessons(m_ScheduleIdTeacher, other_data);
		m_ScheduleTeacherOther->set_model(other_data);
	}
}

void MainWindow::ScheduleTeacherCellData(Gtk::CellRenderer* cell, long int id_hour, long int id_day)
{
	Gtk::CellRendererText *renderer = reinterpret_cast<Gtk::CellRendererText *>(cell);
	renderer->property_text() = DB::DataBase::Instance().GetTimeTableLessonTeacherText(m_ScheduleIdTeacher, id_hour, id_day);
	if(DB::DataBase::Instance().GetTeacherHolydays(m_ScheduleIdTeacher, id_day, id_hour))
	{
		renderer->property_background_gdk() = Gdk::Color("red");
	}
	else
	{
		renderer->property_background_gdk() = Gdk::Color("white");
	}
}

void MainWindow::ScheduleTeacherSelectedOther()
{
	m_ScheduleTeacherSelectedOther = m_ScheduleTeacherOther->get_selection()->get_selected();
}

void MainWindow::ScheduleTeacherCellButtonRelease(long int id_hour, long int id_day, GdkEventButton* event)
{
	switch(event->button)
	{
	case 1: // left button
		if(m_ScheduleTeacherSelectedOther)
		{
			long int lesson_id = m_ScheduleTeacherSelectedOther->get_value(DB::g_IdTextScheme.fId);
			//move lesson to timetable
			//get auditoriums list
			Glib::RefPtr<ORM::Data> aud_list = ORM::Data::create(DB::g_IdTextScheme);
			DB::DataBase::Instance().GetAuditoriumListForLesson(aud_list, lesson_id, id_day, id_hour);
			if(aud_list->children().size() > 0)
			{
				m_ScheduleIdDay = id_day;
				m_ScheduleIdHour = id_hour;
				m_ScheduleMenu.items().erase(m_ScheduleMenu.items().begin(), m_ScheduleMenu.items().end());
				for(Gtk::TreeIter it = aud_list->children().begin(); it != aud_list->children().end(); ++ it)
				{
					Gtk::MenuItem *p_item = Gtk::manage(new Gtk::MenuItem(it->get_value(DB::g_IdTextScheme.fText)));
					m_ScheduleMenu.append(*p_item);
					p_item->signal_activate().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::ScheduleTeacherChooseAud), it->get_value(DB::g_IdTextScheme.fId)));
					p_item->show();
				}
				m_ScheduleMenu.popup(0, event->time);
				//std::cout << "popup size: " << m_ScheduleMenu.items().size() << std::endl;
			}
			else
			{
				std::cout << "Cann't found auditoriums" << std::endl;
			}
		}
		break;
	case 3: // change auditorium or delete
		{
			long int lesson_id = DB::DataBase::Instance().GetTimeTableLessonTeacher(m_ScheduleIdTeacher, id_hour, id_day);
			if(lesson_id != -1)
			{
				//lesson exist
				m_ScheduleIdDay = id_day;
				m_ScheduleIdHour = id_hour;
				Glib::RefPtr<ORM::Data> aud_list = ORM::Data::create(DB::g_IdTextScheme);
				DB::DataBase::Instance().GetAuditoriumListForLesson(aud_list, lesson_id, id_day, id_hour);
				m_ScheduleMenu.items().erase(m_ScheduleMenu.items().begin(), m_ScheduleMenu.items().end());
				if(aud_list->children().size() > 0)
				{
					for(Gtk::TreeIter it = aud_list->children().begin(); it != aud_list->children().end(); ++ it)
					{
						Gtk::MenuItem *p_item = Gtk::manage(new Gtk::MenuItem(it->get_value(DB::g_IdTextScheme.fText)));
						m_ScheduleMenu.append(*p_item);
						p_item->signal_activate().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::ScheduleTeacherChangeAud), lesson_id, it->get_value(DB::g_IdTextScheme.fId)));
						//p_item->show();
					}
					m_ScheduleMenu.append(*Gtk::manage(new Gtk::SeparatorMenuItem()));
					//std::cout << "popup size: " << m_ScheduleMenu.items().size() << std::endl;
				}
				else
				{
					std::cout << "Cann't found auditoriums" << std::endl;
				}
				// add remove item
				Gtk::MenuItem *p_item = Gtk::manage(new Gtk::MenuItem(_("Delete")));
				m_ScheduleMenu.append(*p_item);
				p_item->signal_activate().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::ScheduleTeacherRemoveLesson), lesson_id));
				//p_item->show();
				m_ScheduleMenu.show_all_children();
				m_ScheduleMenu.popup(0, event->time);
			}
		}
		break;
	}
}

void MainWindow::ScheduleTeacherChooseAud(long int aud_id)
{
	if(m_ScheduleTeacherSelectedOther)
	{
		long int lesson_id = m_ScheduleTeacherSelectedOther->get_value(DB::g_IdTextScheme.fId);
		DB::DataBase::Instance().SetLessonIntoTimetable(lesson_id, aud_id, m_ScheduleIdDay, m_ScheduleIdHour);
		ScheduleTeacherChanged();
	}
}

void MainWindow::ScheduleTeacherChangeAud(long int lesson_id, long int aud_id)
{
	DB::DataBase::Instance().RemoveLessonFromTimetable(lesson_id, m_ScheduleIdDay, m_ScheduleIdHour);
	DB::DataBase::Instance().SetLessonIntoTimetable(lesson_id, aud_id, m_ScheduleIdDay, m_ScheduleIdHour);
	ScheduleTeacherChanged();
}

void MainWindow::ScheduleTeacherRemoveLesson(long int lesson_id)
{
	DB::DataBase::Instance().RemoveLessonFromTimetable(lesson_id, m_ScheduleIdDay, m_ScheduleIdHour);
	ScheduleTeacherChanged();
}

void MainWindow::TeachingLessonGroupExpose()
{
	Glib::RefPtr<ORM::Data> data = ORM::Data::create(DB::g_IdTextScheme);
	DB::DataBase::Instance().GetSubgroupsList(data);
	m_ComboBoxTeachingLesson->set_model(data);
}

void MainWindow::TeachingLessonGroupChanged()
{
	Gtk::TreeIter iter = m_ComboBoxTeachingLesson->get_active();
	if(iter)
	{
		m_TeachingLesson->SetSubgroup(iter->get_value(DB::g_IdTextScheme.fId));
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
	DB::DataBase::Instance().CleanTimeTable();
	ScheduleGroupChanged();
}

void MainWindow::OnScheduleRun()
{
	GraphForTime graph;
}

void MainWindow::OnException()
{
	try
	{
		throw;
	}
	catch(std::exception& e)
	{
		//m_StatusBar->remove_all_messages();
		std::cerr << e.what() << std::endl;
		m_StatusBar->push(e.what());
	}
	catch(Glib::Exception& e)
	{
		//m_StatusBar->remove_all_messages();
		std::cerr << e.what() << std::endl;
		m_StatusBar->push(e.what());
	}
}

