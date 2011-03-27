#include "celleditableforeign.h"
#include "db/db.h"

Glib::ustring CellEditableForeign::get_text() const
{
	return DB::DataBase::Instance().GetTextById(m_Table, m_Field, m_Id);
}

long CellEditableForeign::get_id() const
{
	return m_Id;
}

void CellEditableForeign::set_id(long id)
{
	m_Id = id;
}

