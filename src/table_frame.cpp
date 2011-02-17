#include <gtkmm/label.h>
#include "table_frame.h"
#include "db/models.h"

TableFrame::TableFrame(DB::DataBase &db)
	:m_DB(db),
	m_Auditoriums(ORM::Data::create(DB::g_ModelAud)),
	m_Days(ORM::Data::create(DB::g_ModelEntity)),
	m_Hours(ORM::Data::create(DB::g_ModelEntity)),
	m_LessonRecords(ORM::Data::create(DB::g_ModelLessonRecords))
{
	pack_start(m_SWTable, true, true);
	pack_end(m_SWUnsetLessons, false, true);

	m_DB.ListEntityAud(DB::g_Auditoriums, m_Auditoriums);
	m_DB.ListEntity(DB::g_Days, m_Days);
	m_DB.ListEntity(DB::g_Hours, m_Hours);

/*	m_UnsetLessons.pack_start(*Gtk::manage(new TableCell("D", "E", "F", 1)));
	m_UnsetLessons.pack_start(*Gtk::manage(new TableCell("D", "E", "F", 1)));
	m_UnsetLessons.pack_start(*Gtk::manage(new TableCell("D", "E", "F", 1)));
	m_UnsetLessons.pack_start(*Gtk::manage(new TableCell("фыфавы", "ыпрварвар", "вролддпд", 3)));*/

/*	m_SWTable.add(m_Table);
	m_SWTable.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	m_SWUnsetLessons.add(m_UnsetLessons);
	m_SWUnsetLessons.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC); */

	show_all();
}

