#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/notebook.h>
#include <gtkmm/actiongroup.h>
#include <gtkmm/builder.h>
#include <gtkmm/comboboxentry.h>
#include "id_text_scheme.h"
#include "listview.h"
#include "sheet.h"
#include "plan_sheet.h"
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

	void OnScheduleNew();
	void OnScheduleCopy();
	void OnScheduleDelete();

	void OnException();

	void ShowAllEntities();

	bool OnFocusIn(GdkEventFocus* event, LineEditable *line_editor);
	bool OnFocusOut(GdkEventFocus* event);

	ListView* AddListView(const Glib::ustring& name, const ORM::Table& scheme);

	// Days -> Double week
	Gtk::CheckButton *m_DoubleWeek;
	void WeekToggle();

	// Loadings -> Holydays
	Gtk::ComboBoxEntry *m_HolydaysCategory;
	Gtk::ComboBoxEntry *m_HolydaysObjectList;
	Sheet *m_SheetHolydays;
	void SwitchHolydayCategory();
	void SwitchHolydayObject();
	void HolydaysCellData(Gtk::CellRenderer *cell, long int row, long int column);
	void HolydaysButtonRelease(long int row, long int column, GdkEventButton* event);

	// Loadings -> Plan
	PlanSheet *m_PlanSheet;
	Gtk::ComboBox *m_ComboBoxPlanSpeciality;
	bool PlanSpecialitiesExpose(GdkEventExpose* event);
	void PlanSpecialitiesChanged();
	void PlanSpecialitiesCellData(Gtk::CellRenderer *cell, long int id_branch, long int id_lesson_type);
	void PlanSpecialitiesButtonRelease(long int row, long int column, GdkEventButton* event);

	// Loadings -> Teaching lesson
	Gtk::TreeView *m_TeachingLesson;
	Gtk::ComboBox *m_ComboBoxTeachingLesson;
	bool TeachingLessonGroupExpose(GdkEventExpose* event);
	void TeachingLessonGroupChanged();

	// Schedule -> Group
	Sheet *m_ScheduleGroup;
	Gtk::ComboBox *m_ComboBoxScheduleGroup;
	bool ScheduleGroupExpose(GdkEventExpose* event);
	void ScheduleGroupChanged();

	void ScheduleGroupCellData(Gtk::CellRenderer* cell, long int id_hour, long int id_day);

	Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;

	LineEditable *m_pCurrentLineEditor;

	Gtk::Statusbar *m_StatusBar;

	IdTextScheme m_ComboScheme;
};

#endif

