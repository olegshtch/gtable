#include "primary_key.h"
#include "table.h"

ORM::Field<ORM::PrimaryKey>::Field(const ORM::Table& tbl)
	:ORM::FieldBase("id_" + tbl.GetTableName())
{
}

