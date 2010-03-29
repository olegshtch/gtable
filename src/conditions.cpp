#include "conditions.h"

Conditions::Conditions(DB::DataBase &db)
	:m_DB(db),
	A(0), D(0), H(0), T(0), G(0), L(0)
{
	A = m_DB.GetEntitiesCount(DB::g_Auditoriums);
	D = m_DB.GetEntitiesCount(DB::g_Days);
	H = m_DB.GetEntitiesCount(DB::g_Hours);
	G = m_DB.GetEntitiesCount(DB::g_Groups);
	T = m_DB.GetEntitiesCount(DB::g_Teachers);
	L = m_DB.GetEntitiesCount(DB::g_Lessons);
	if(A && D && H && G && T && L)
	{
		ids_a.resize(A);
		ids_d.resize(D);
		ids_h.resize(H);
		ids_g.resize(G);
		ids_t.resize(T);
		ids_l.resize(L);
		m_DB.GetEntitiesIDs(DB::g_Auditoriums, &ids_a);
		m_DB.GetEntitiesIDs(DB::g_Days, &ids_d);
		m_DB.GetEntitiesIDs(DB::g_Hours, &ids_h);
		m_DB.GetEntitiesIDs(DB::g_Groups, &ids_g);
		m_DB.GetEntitiesIDs(DB::g_Teachers, &ids_t);
		m_DB.GetEntitiesIDs(DB::g_Lessons, &ids_l);
	}
	else
	{
		throw Glib::Error(1, 0, "Zero entity.");
	}
}

Conditions::~Conditions()
{
}

