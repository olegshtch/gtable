#include "conditions.h"

Conditions::Conditions(DB::DataBase &db)
	:m_DB(db)
{
	m_DB.GetEntitiesCount(DB::g_Auditoriums);
	m_DB.GetEntitiesCount(DB::g_Days);
	m_DB.GetEntitiesCount(DB::g_Hours);
	m_DB.GetEntitiesCount(DB::g_Groups);
	m_DB.GetEntitiesCount(DB::g_Teachers);
	m_DB.GetEntitiesCount(DB::g_Lessons);
}

Conditions::~Conditions()
{
}

