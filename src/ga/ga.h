#ifndef _GA_H_
#define _GA_H_

#include <stdint.h>
#include "../db/db.h"
#include "individual.h"

// Занятие
struct Task
{
	long g; // индекс группы
	long t; // индекс преподавателя
	long l; // индекс занятия
	uint8_t hours; // количество часов

	Task(long g_, long t_, long l_, uint8_t hours_)
		:g(g_), t(t_), l(l_), hours(hours_)
	{
	}
};

namespace GA
{
	void Solve(DB::DataBase &db);
	void Run(const std::vector<bool> &less_aud, const std::vector<std::vector<Task>::const_iterator> &table_base, const std::vector<bool> &multi_aud, size_t A, size_t D, size_t H);
	bool Loop(std::vector<Individual> *population);
}

#endif

