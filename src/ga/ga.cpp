#include <fstream>
#include <algorithm>
#include "ga.h"
#include "individual.h"
#include "../shared.h"

const size_t POPULATION_SIZE = 75;
const size_t MAX_STABLE_LOOPS = 200;

GA::GA()
	:m_StaySteps(MAX_STABLE_LOOPS)
{
	std::vector<size_t> multi_aud_count;
	std::vector<size_t> single_aud_count;
	std::vector<GraphForTime::ItemColoring> coloring = m_Graph.Coloring(&multi_aud_count, &single_aud_count);
	m_Population.resize(POPULATION_SIZE, Individual(m_Graph, coloring, multi_aud_count, single_aud_count));
}

bool GA::Loop()
{
	long min_fitness = m_Population[0].GetFitness();
	std::vector<Individual> new_population;

	std::cout << "Mutations" << std::endl;
	size_t i;
	// заполнение результатами мутации
	for(i = 0; i < (POPULATION_SIZE - 1); ++ i)
	{
		//CheckPopulation(m_Population);
		//m_Population[i].CheckSelf();
		Individual ind = m_Population[i];
		//m_Population[i].CheckSelf();
		//ind.CheckSelf();
		if(ind.Mutation())
		{
			// мутация прошла успешно
			new_population.push_back(ind);
		}
	}
#if 0
	// заполнение результатами скрещивания
	for(i = new_population.size(); i < (POPULATION_SIZE - 1); ++ i)
	{
		Individual ind1 = m_Population[rand() % m_Population.size()];
		Individual ind2 = m_Population[rand() % m_Population.size()];
		if((rand() % 10 == 0)
			&& ind1.Crossover(ind2))
		{
			// скрещивание прошло успешно
			new_population.push_back(ind1);
		}
	}
#endif
	// заполнение оставшихся мест (хотя бы 1) родителями
	for(i = 0; new_population.size() < POPULATION_SIZE; ++ i)
	{
		new_population.push_back(m_Population[i]);
	}
	// сортировка
	std::sort(new_population.begin(), new_population.end());
	//std::cout << "Check after sort" << std::endl;
	//CheckPopulation(new_population);
	m_Population = new_population;
	//std::cout << "Check after assign" << std::endl;
	//CheckPopulation(m_Population);
	if(min_fitness > m_Population[0].GetFitness())
	{
		m_StaySteps = MAX_STABLE_LOOPS;
		return true;
	}
	else if(m_StaySteps > 0)
	{
		-- m_StaySteps;
		std::cout << "steps: " << m_StaySteps << " fitness:" << m_Population[0].GetFitness() << std::endl;
		return true;
	}
	m_Population[0].Output();
	return false;
}

void GA::CheckPopulation(const std::vector<Individual>& population)
{
	for(size_t i = 0; i < population.size(); ++ i)
	{
		population[i].CheckSelf();
	}
}

