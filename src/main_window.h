#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/notebook.h>
#include <gtkmm/actiongroup.h>
#include <gtkmm/builder.h>
#include <gtkmm/comboboxentry.h>
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
	TeachingLesson *m_TeachingLesson;
	Gtk::ComboBox *m_ComboBoxTeachingLesson;
	bool TeachingLessonGroupExpose(GdkEventExpose* event);
	void TeachingLessonGroupChanged();

	// Schedule -> Group
	Sheet *m_ScheduleGroup;
	Gtk::ComboBox *m_ComboBoxScheduleGroup;
	Gtk::TreeView *m_ScheduleGroupOther;
	Gtk::TreeIter m_ScheduleGroupSelectedOther;
	long int m_ScheduleIdGroup;
	long int m_ScheduleIdDay;
	long int m_ScheduleIdHour;
	Gtk::Menu m_ScheduleMenu;
	bool ScheduleGroupExpose(GdkEventExpose* event);
	void ScheduleGroupChanged();
	void ScheduleGroupCellData(Gtk::CellRenderer* cell, long int id_hour, long int id_day);
	void ScheduleGroupSelectedOther();
	void ScheduleGroupCellButtonRelease(long int id_hour, long int id_day, GdkEventButton* event);
	void ScheduleGroupChooseAud(long int aud_id);
	void ScheduleGroupChangeAud(long int lesson_id, long int aud_id);
	void ScheduleGroupRemoveLesson(long int lesson_id);

	Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;

	LineEditable *m_pCurrentLineEditor;
	std::vector<LineEditable*> m_LineEditors;

	Gtk::Statusbar *m_StatusBar;
};

#endif

