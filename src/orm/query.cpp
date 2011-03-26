#include "connection.h"

ORM::QueryBase::~QueryBase()
{
	if(m_Data)
	{
		m_DB.SQLExec(m_Query, m_Data);
	}
	else
	{
		m_DB.SQLExec0(m_Query);
	}
}

