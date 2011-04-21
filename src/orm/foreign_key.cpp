#include "foreign_key.h"
#include "table.h"

ORM::Field<ORM::ForeignKey>::Field(const Glib::ustring& field_name, const Table& tbl, bool not_null)
	:ORM::Field<long int>(field_name),
	m_ForeignTable(tbl.GetTableName()),
	m_NotNull(not_null)
{
}

ORM::Field<ORM::ForeignKey>::Field(const ORM::Table& tbl, bool not_null)
	:ORM::Field<long int>("id_" + tbl.GetTableName()),
	m_ForeignTable(tbl.GetTableName()),
	m_NotNull(not_null)
{
}

