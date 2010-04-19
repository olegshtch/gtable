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
		size_t a, d, h, g, t, l;
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
		for(a = 0; a < A; a ++)
		{
			for(l = 0; l < L; l ++)
			{
				array_AL[a][l] = m_DB.IsLinkBetween(DB::g_LessonsAuditoriums, ids_a[a], ids_l[l]);
			}
		}
		std::vector<std::vector<size_t> > array_GL; // store teachers for group and lesson
		array_GL.resize(G, std::vector<size_t>(L));
		for(g = 0; g < G; g ++)
		{
			for(l = 0; l < L; l ++)
			{
				std::vector<size_t>::iterator it = std::find(ids_t.begin(), ids_t.end(), m_DB.GetTForGL(ids_g[g], ids_l[l]));
				if(it != ids_t.end())
				{
					array_GL[g][l] = *it;
				}
				else
				{
					array_GL[g][l] = T; // group have no this lesson
				}
			}
		}
		// exclude unused coordinates
		//  count size of coordinate array
		size_t full_count = A * D * H * G * T * L;
		size_t count_ALG = 0;
		std::cout << "Full count = " << full_count << std::endl;
		for(a = 0; a < A; a ++)
		{
			for(l = 0; l < L; l ++)
			{
				if(array_AL[a][l])
				{
					// check for group and lesson
					for(g = 0; g < G; g ++)
					{
						if(array_GL[g][l] < T)
						{
							// group can have lesson in this auitory;
							count_ALG ++;
						}
					}
				}
			}
		}
		size_t N = count_ALG * D * H;
		std::cout << "N = " << N << std::endl;
		std::cout << "Optimize level = " << (1.0 - static_cast<float>(N)/static_cast<float>(full_count)) * 100.0 << "%" << std::endl;
		
	}
	else
	{
		throw Glib::Error(1, 0, "Zero entity.");
	}
}

Conditions::~Conditions()
{
}

