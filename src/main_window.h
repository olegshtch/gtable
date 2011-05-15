#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/notebook.h>
#include <gtkmm/actiongroup.h>
#include <gtkmm/builder.h>
#include <gtkmm/comboboxentry.h>
#include <glibmm/dispatcher.h>
#include "db/id_text_scheme.h"
#include "listview.h"
#include "sheet.h"
#include "plan_sheet.h"
#include "teaching_lesson.h"
#include "db/db.h"

class MainWindow : public Gtk::Window
{
public:
	MainWindow(GtkWindow *cobject, const Glib::RefPtr<Gtk::Builder>& builder);
	virtual ~MainWindow();
private:
	Glib::RefPtr<Gtk::Builder> m_refBuilder;
	Glib::RefPtr<Glib::MainContext> m_refMainContext;
	Glib::Dispatcher m_SolvingDispatcher;

	void OnSolvingEmit();

	void OnNew();
	void OnOpen();
	void OnSave();
	void OnQuit();
	void OnAbout();

	void OnRun();
	void OnEdit();

	void OnAppend();
	void OnDelete();

	void OnScheduleNew();
	void OnScheduleCopy();
	void OnScheduleDelete();
	void OnScheduleRun();

	void Solving();

	void OnException();

	void ShowAllEntities();

	bool OnFocusIn(GdkEventFocus* event, LineEditable *line_editor);
	bool OnFocusOut(GdkEventFocus* event);

	ListView* AddListView(const Glib::ustring& name, const ORM::Table& scheme);

	// Days -> Double week
	Gtk::CheckButton *m_DoubleWeek;
	void WeekToggle();

	// Loadings -> Holydays
	Gtk::ComboBox *m_HolydaysCategory;
	Gtk::ComboBoxEntry *m_HolydaysObjectList;
	Glib::RefPtr<ORM::Data> m_HolydaysCategoryList;
	long int m_HolydaysIdCategory;
	Sheet *m_SheetHolydays;
	void SwitchHolydayCategory();
	void SwitchHolydayObject();
	void HolydaysCellData(Gtk::CellRenderer *cell, long int row, long int column);
	void HolydaysButtonRelease(long int row, long int column, GdkEventButton* event);

	// Loadings -> Plan
	PlanSheet *m_PlanSheet;
	Gtk::ComboBox *m_ComboBoxPlanSpeciality;
	void PlanSpecialitiesExpose();
	void PlanSpecialitiesChanged();
	void PlanSpecialitiesCellData(Gtk::CellRenderer *cell, long int id_branch, long int id_lesson_type);
	void PlanSpecialitiesButtonRelease(long int row, long int column, GdkEventButton* event);

	// Loadings -> Teaching lesson
	TeachingLesson *m_TeachingLesson;
	Gtk::ComboBox *m_ComboBoxTeachingLesson;
	void TeachingLessonGroupExpose();
	void TeachingLessonGroupChanged();

	// Schedule -> Group
	Sheet *m_ScheduleGroup;
	Gtk::ComboBox *m_ComboBoxScheduleGroup;
	Gtk::TreeView *m_ScheduleGroupOther;
	Gtk::TreeIter m_ScheduleGroupSelectedOther;
	long int m_ScheduleIdGroup;
	void ScheduleGroupExpose();
	void ScheduleGroupChanged();
	void ScheduleGroupCellData(Gtk::CellRenderer* cell, long int id_hour, long int id_day);
	void ScheduleGroupSelectedOther();
	void ScheduleGroupCellButtonRelease(long int id_hour, long int id_day, GdkEventButton* event);
	void ScheduleGroupChooseAud(long int aud_id);
	void ScheduleGroupChangeAud(long int lesson_id, long int aud_id);
	void ScheduleGroupRemoveLesson(long int lesson_id);

	// Schedule -> Teacher
	Sheet *m_ScheduleTeacher;
	Gtk::ComboBox *m_ComboBoxScheduleTeacher;
	Gtk::TreeView *m_ScheduleTeacherOther;
	Gtk::TreeIter m_ScheduleTeacherSelectedOther;
	long int m_ScheduleIdTeacher;
	void ScheduleTeacherExpose();
	void ScheduleTeacherChanged();
	void ScheduleTeacherCellData(Gtk::CellRenderer* cell, long int id_hour, long int id_day);
	void ScheduleTeacherSelectedOther();
	void ScheduleTeacherCellButtonRelease(long int id_hour, long int id_day, GdkEventButton* event);
	void ScheduleTeacherChooseAud(long int aud_id);
	void ScheduleTeacherChangeAud(long int lesson_id, long int aud_id);
	void ScheduleTeacherRemoveLesson(long int lesson_id);

	// Schedule -> Auditorium
	Sheet *m_ScheduleAuditorium;
	Gtk::ComboBox *m_ComboBoxScheduleAuditorium;
	Gtk::TreeView *m_ScheduleAuditoriumOther;
	Gtk::TreeIter m_ScheduleAuditoriumSelectedOther;
	long int m_ScheduleIdAuditorium;
	void ScheduleAuditoriumExpose();
	void ScheduleAuditoriumChanged();
	void ScheduleAuditoriumCellData(Gtk::CellRenderer* cell, long int id_hour, long int id_day);
	void ScheduleAuditoriumSelectedOther();
	void ScheduleAuditoriumCellButtonRelease(long int id_hour, long int id_day, GdkEventButton* event);

	long int m_ScheduleIdDay;
	long int m_ScheduleIdHour;
	Gtk::Menu m_ScheduleMenu;

	Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;

	LineEditable *m_pCurrentLineEditor;
	std::vector<LineEditable*> m_LineEditors;

	Gtk::Statusbar *m_StatusBar;
};

#endif

