#include <algorithm>
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
		// get array by auditoriums and lessons
		std::vector<std::vector<bool> > array_AL;
		array_AL.resize(A, std::vector<bool>(L));
		for(size_t a = 0; a < A; a ++)
		{
			for(size_t l = 0; l < L; l ++)
			{
				array_AL[a][l] = m_DB.IsLinkBetween(DB::g_LessonsAuditoriums, ids_a[a], ids_l[l]);
			}
		}
		std::vector<std::vector<size_t> > array_GL; // store teachers for group and lesson
		array_GL.resize(G, std::vector<size_t>(L));
		for(size_t g = 0; g < A; g ++)
		{
			for(size_t l = 0; l < L; l ++)
			{
				array_GL[g][l] = *std::find(ids_t.begin(), ids_t.end(), m_DB.GetTForGL(ids_g[g], ids_l[l]));
			}
		}
	}
	else
	{
		throw Glib::Error(1, 0, "Zero entity.");
	}
}

Conditions::~Conditions()
{
}

