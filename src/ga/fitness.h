#ifndef _GA_FITNESS_H_
#define _GA_FITNESS_H_

#include "individual.h"

struct Fitness
{
	Fitness(const std::vector<Individual>::const_iterator& ind_)
		:ind(ind_)
	{
	}
private:

	std::vector<Individual>::const_iterator ind;
};


#endif

