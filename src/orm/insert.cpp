#include "insert.h"

void ORM::InsertBase::Values(const Gtk::TreeIter& row)
{
	m_Query += " (";
	for(size_t i = 1; i < m_Table.size(); ++ i)
	{
		m_Query += m_Table.GetField(i).GetFieldName();
		if(i < m_Table.size() - 1)
		{
			m_Query += ",";
		}
	}
	m_Query += ") VALUES (";
	for(size_t i = 1; i < m_Table.size(); ++ i)
	{
		m_Query += m_Table.GetField(i).GetStrValue(row);
		if(i < m_Table.size() - 1)
		{
			m_Query += ",";
		}
	}
	m_Query += ")";
}

