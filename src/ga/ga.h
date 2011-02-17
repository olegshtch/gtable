#ifndef _GA_H_
#define _GA_H_

#include <stdint.h>
#include "../db/db.h"

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

class Individual;
class ADH;

class GA
{
	friend class Individual;
	friend class ADH;
public:
	GA(DB::DataBase &db_);
	~GA()
	{
	}

	//void Solve(DB::DataBase &db);
	void Run();
	bool Loop(std::vector<Individual> *population);
private:
	DB::DataBase &db;

	std::vector<bool> less_aud; // адресация [l * A + a]
	std::vector<Task> tasks;
	std::vector<std::vector<Task>::const_iterator> table_base;
	std::vector<bool> multi_aud;
	static size_t A, D, H, G, T, L;
	std::vector<size_t> ids_a, ids_d, ids_h, ids_g, ids_t, ids_l;
};

#endif

