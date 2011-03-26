#include "foreign_key.h"
#include "table.h"

ORM::Field<ORM::ForeignKey>::Field(const ORM::Table& tbl)
	:ORM::Field<ORM::PrimaryKey>(tbl),
	m_ForeignTable(tbl.GetTableName())
{
}

