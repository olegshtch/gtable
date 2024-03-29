#include "insert.h"
#include "subquery.h"

void ORM::InsertBase::Values(const Gtk::TreeIter& row)
{
	m_Query = "INSERT INTO " + m_Table.GetTableName();
	m_Query += " (";
	for(size_t i = 0; i < m_Fields.size(); ++ i)
	{
		m_Query += m_Fields[i]->GetSmallFieldName();
		if(i < m_Fields.size() - 1)
		{
			m_Query += ",";
		}
	}
	m_Query += ") VALUES (";
	for(size_t i = 0; i < m_Fields.size(); ++ i)
	{
		m_Query += m_Fields[i]->GetStrValue(row);
		if(i < m_Fields.size() - 1)
		{
			m_Query += ",";
		}
	}
	m_Query += ")";
}

void ORM::InsertBase::Select(const ORM::Subquery& subquery)
{
	m_Query = "INSERT INTO " + m_Table.GetTableName();
	m_Query += " (";
	for(size_t i = 0; i < m_Fields.size(); ++ i)
	{
		m_Query += m_Fields[i]->GetSmallFieldName();
		if(i < m_Fields.size() - 1)
		{
			m_Query += ",";
		}
	}
	m_Query += ") " + subquery.GetQuery();
}

