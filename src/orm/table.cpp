#include "table.h"
#include "connection.h"

std::set<const ORM::Table*> *ORM::Table::s_Tables = NULL;

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
	for(std::vector<Glib::ustring>::const_iterator constraint = m_Constraints.begin(); constraint != m_Constraints.end(); ++ constraint)
	{
		query += (*constraint);
		query += ",";
	}
	query.replace(query.size() - 1, 1, 1, ')');
	return query;
}

void ORM::Table::InitTables(ORM::Connection &db)
{
	for(std::set<const Table*>::const_iterator it = s_Tables->begin(); it != s_Tables->end(); ++it)
	{
		db.CreateTable(**it, true);
	}
}


