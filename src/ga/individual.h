#ifndef _GA_INDIVIDUAL_H_
#define _GA_INDIVIDUAL_H_

#include <vector>
#include <iostream>
#include <cstdlib>
#include "graph.h"

// особь
class Individual
{
public:
	Individual(const GraphForTime& graph, const std::vector<GraphForTime::ItemColoring>& coloring, const std::vector<size_t> &multi_aud_count, const std::vector<size_t>& single_aud_count)
		:m_Graph(graph)
		,m_Coloring(coloring)
		,m_MultiAudCount(multi_aud_count)
		,m_SingleAudCount(single_aud_count)
	{
		m_Fitness = FitnessCalculate();
	}

	Individual(const Individual& ind)
		:m_Graph(ind.m_Graph)
		,m_Coloring(ind.m_Coloring)
		,m_MultiAudCount(ind.m_MultiAudCount)
		,m_SingleAudCount(ind.m_SingleAudCount)
		,m_Fitness(ind.m_Fitness)
	{
		//ind.CheckSelf();
		//m_Coloring = ind.m_Coloring;
		//m_MultiAudCount = ind.m_MultiAudCount;
		//m_SingleAudCount = ind.m_SingleAudCount;
		//m_Fitness = ind.m_Fitness;
		//ind.CheckSelf();
		//CheckSelf();
	}

	~Individual()
	{
	}

	Individual& operator=(const Individual& ind)
	{
		if(this != &ind)
		{
			m_Coloring = ind.m_Coloring;
			m_MultiAudCount = ind.m_MultiAudCount;
			m_SingleAudCount = ind.m_SingleAudCount;
			m_Fitness = ind.m_Fitness;
		}
		return *this;
	}

	bool operator<(const Individual& ind) const
	{
		return m_Fitness < ind.m_Fitness;
	}
	
	long GetFitness() const
	{
		return m_Fitness;
	}

	bool Mutation();
	bool Crossover(const Individual& ind);

	void Output() const;

	void CheckSelf() const;
private:
	long FitnessCalculate() const;

	const GraphForTime& m_Graph;
	std::vector<GraphForTime::ItemColoring> m_Coloring;
	std::vector<size_t> m_MultiAudCount; // по индексу цвета
	std::vector<size_t> m_SingleAudCount; // по индексу цвета
	long m_Fitness;
};


#endif

