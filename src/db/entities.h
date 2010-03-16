#ifndef _DB_ENTITES_H_
#define _DB_ENTITES_H_

namespace DB
{
	struct Entity
	{
		const char* m_TableName;
		const bool m_SortByName;
		Entity(const char* table_name, bool sort_by_name)
			:m_TableName(table_name), m_SortByName(sort_by_name) {}
	};

	extern const Entity g_Auditoriums;
	extern const Entity g_Days;
	extern const Entity g_Hours;
	extern const Entity g_Groups;
	extern const Entity g_Teachers;
	extern const Entity g_Lessons;

}

#endif

