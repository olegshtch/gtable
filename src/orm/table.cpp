#include "table.h"

std::set<const ORM::Table*> ORM::Table::s_Tables;

Glib::ustring ORM::Table::GetSqlCreateString() const
{
	Glib::ustring query = "CREATE TABLE ";
	query += m_TableName;
	query += "(";
	for(std::vector<const FieldBase*>::const_iterator it = m_Fields.begin(); it != m_Fields.end(); ++ it)
	{
		query += (*it)->GetDefinition();
		query += ",";
	}
	query.replace(query.size() - 1, 1, 1, ')');
	return query;
}

