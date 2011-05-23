#ifndef _GA_H_
#define _GA_H_

#ifdef WIN32
typedef unsigned char uint8_t;
#else
#include <stdint.h>
#endif
#include <vector>

#include "graph.h"
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

class GA
{
public:
	GA();
	~GA()
	{
	}

	bool Loop();

private:
	void CheckPopulation(const std::vector<Individual>& population);

	const GraphForTime m_Graph;
	std::vector<Individual> m_Population;
	size_t m_StaySteps;
};

#endif

