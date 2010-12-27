#ifndef _GA_FITNESS_H_
#define _GA_FITNESS_H_

#include "individual.h"

struct Fitness
{
	Fitness(const std::vector<Individual>::const_iterator& ind_, size_t errors_, size_t quality_)
		:ind(ind_), errors(errors_), quality(quality_)
	{
	}
private:
	std::vector<Individual>::const_iterator ind;
	size_t errors;
	size_t quality;
};


#endif

