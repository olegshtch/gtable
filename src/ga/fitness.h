#ifndef _GA_FITNESS_H_
#define _GA_FITNESS_H_

#include "individual.h"

struct Fitness
{
	Fitness(const std::vector<Individual>::const_iterator& ind_, size_t errors_, size_t quality_)
		:ind(ind_), errors(errors_), quality(quality_)
	{
	}

	bool operator<(const Fitness& other) const
	{
		if(errors < other.errors)
		{
			return true;
		}
		else
		{
			if(errors > other.errors)
			{
				return false;
			}
		}
		return quality < other.quality;
	}

	const Individual& GetIndividual() const
	{
		return *ind;
	}
	std::vector<Individual>::const_iterator GetIter() const
	{
		return ind;
	}
private:
	std::vector<Individual>::const_iterator ind;
public:
	size_t errors;
	size_t quality;
};


#endif

