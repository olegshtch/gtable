#if 0
#ifndef _DB_LINKS_H_
#define _DB_LINKS_H_

#include "entities.h"

namespace DB
{
	struct Link_N2N : public Entity
	{
		const Entity &m_Entity1, &m_Entity2;
		Link_N2N(const char* table_name, const Entity &ent1, const Entity &ent2)
			:Entity(table_name, true), m_Entity1(ent1), m_Entity2(ent2) {}
	};

	struct Link_TeachPlan : public Link_N2N
	{
		const Link_N2N &m_Entity2;
		Link_TeachPlan(const char* table_name, const Entity &ent1, const Link_N2N &ent2)
			:Link_N2N(table_name, ent1, reinterpret_cast<const Entity&>(ent2)), m_Entity2(ent2) {}
	};

	extern const Link_N2N g_LessonsAuditoriums;
	extern const Link_N2N g_TeachersLessons;
	extern const Link_N2N g_OrderLessons;
	extern const Link_TeachPlan g_TeachPlan;
}

#endif
#endif
