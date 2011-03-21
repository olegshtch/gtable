#include "select.h"
#include "connection.h"

ORM::SelectBase::~SelectBase()
{
	m_DB.SQLExec("SELECT " + m_Query, m_Data);
}

