#include <gtkmm/label.h>
#include "table_frame.h"
#include "db/models.h"

TableFrame::TableFrame(DB::DataBase &db)
	:m_DB(db),
	m_Table(1, 1),
	m_Auditoriums(ORM::Table::create(DB::g_ModelAud)),
	m_Days(ORM::Table::create(DB::g_ModelEntity)),
	m_Hours(ORM::Table::create(DB::g_ModelEntity)),
	m_LessonRecords(ORM::Table::create(DB::g_ModelLessonRecords))/*,
	m_TableCell("A", "B", "C")*/
{
	pack_start(m_SWTable, true, true);
	pack_end(m_SWUnsetLessons, false, true);

	m_DB.ListEntityAud(DB::g_Auditoriums, m_Auditoriums);
	m_DB.ListEntity(DB::g_Days, m_Days);
	m_DB.ListEntity(DB::g_Hours, m_Hours);
	const size_t aud_size = m_Auditoriums->children().size();
	const size_t days_size = m_Days->children().size();
	const size_t hours_size = m_Hours->children().size();
	m_Table.resize(aud_size + 2, days_size * hours_size + 1);
	
	// set labels
	for(size_t a = 0; a < aud_size; a ++)
	{
		m_Table.attach(*Gtk::manage(new Gtk::Label(m_Auditoriums->children()[a][DB::g_ModelAud.name])), 2 + a, 3 + a, 0, 1);
	}
	for(size_t d = 0; d < days_size; d ++)
	{
		for(size_t h = 0; h < hours_size; h ++)
		{
			m_Table.attach(*Gtk::manage(new Gtk::Label(m_Hours->children()[h][DB::g_ModelEntity.name])), 1, 2, 1 + hours_size * d + h, 2 + hours_size * d + h);
			for(size_t a = 0; a < aud_size; a ++)
			{
				m_Table.attach(*Gtk::manage(new TableCell), 2 + a, 3 + a, 1 + hours_size * d + h, 2 + hours_size * d + h);
			}
		}
		m_Table.attach(*Gtk::manage(new Gtk::Label(m_Days->children()[d][DB::g_ModelEntity.name])), 0, 1, 1 + hours_size * d, 1 + hours_size * (d + 1));
	}

	/*m_Table.attach(m_TableCell, 2, aud_size + 2, 1, days_size * hours_size + 1);*/

	m_DB.ListLessonRecords(m_LessonRecords);
	for(Gtk::TreeIter record = m_LessonRecords->children().begin(); record != m_LessonRecords->children().end(); record ++)
	{
		m_UnsetLessons.pack_start(*Gtk::manage(new TableCell(
			record->get_value(DB::g_ModelLessonRecords.t_name),
			record->get_value(DB::g_ModelLessonRecords.l_name),
			record->get_value(DB::g_ModelLessonRecords.g_name),
			record->get_value(DB::g_ModelLessonRecords.hours))),
			false, true);
	}

/*	m_UnsetLessons.pack_start(*Gtk::manage(new TableCell("D", "E", "F", 1)));
	m_UnsetLessons.pack_start(*Gtk::manage(new TableCell("D", "E", "F", 1)));
	m_UnsetLessons.pack_start(*Gtk::manage(new TableCell("D", "E", "F", 1)));
	m_UnsetLessons.pack_start(*Gtk::manage(new TableCell("фыфавы", "ыпрварвар", "вролддпд", 3)));*/

	m_SWTable.add(m_Table);
	m_SWTable.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	m_SWUnsetLessons.add(m_UnsetLessons);
	m_SWUnsetLessons.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);

	show_all();
}

