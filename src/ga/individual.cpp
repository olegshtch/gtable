#include <cstdlib>
#include "../shared.h"
#include "individual.h"

const long UNCOLORED_ITEM = 10;
const long UNORDERED_TYPES = 1;

void Individual::Output() const
{
	m_Graph.StoreTimetable(m_Coloring);
}

bool Individual::Mutation()
{
	//выбрать случайное занятие
	size_t lesson_index = rand() % m_Graph.Items().size();
	
	std::vector<bool> possible_colors(m_Graph.Colors().size(), true);
	if(m_Coloring[lesson_index].color > 0)
	{
		possible_colors[m_Coloring[lesson_index].color] = false;
	}
#if 0
	if((m_Coloring[lesson_index].color < 0)
		|| (m_Coloring[lesson_index].color >= m_Graph.Colors().size()))
	{
		THROW("Color index impossble");
	}
	if(m_Coloring[lesson_index].a != -1) // аудитории не трогаем
	{
		THROW("Auditorium set");
	}
#endif
	size_t i;
	for(i = 0; i < m_Graph.Items().size(); ++ i) //для каждого занятия
	{
		if(m_Graph.Links()[lesson_index][i])
		{
#if 0
			if((m_Coloring[i].color < 0)
				|| (m_Coloring[i].color >= m_Graph.Colors().size())) // все вершины должны быть закрашены
			{
				THROW("Color index impossble");
			}
			if(m_Coloring[i].a != -1) // аудитори не трогаем
			{
				THROW("Auditorium set");
			}
#endif
			//соседняя вершина
			if(m_Coloring[i].color >= 0)
			{
				possible_colors[m_Coloring[i].color] = false;
			}
		}
	}
	std::vector<int> colors_list;
	colors_list.reserve(m_Graph.Colors().size());
	for(i = 0; i < m_Graph.Colors().size(); ++ i)
	{
		if(m_Graph.Items()[lesson_index].m)
		{
			if(m_MultiAudCount[i] == 0)
			{
				possible_colors[i] = false;
			}
		}
		else
		{
			if(m_SingleAudCount[i] == 0)
			{
				possible_colors[i] = false;
			}
		}

		// формирование сиска доступных цветов
		if((m_Graph.Holydays().count(std::make_pair(lesson_index, i)) == 0)
			&& possible_colors[i])
		{
			colors_list.push_back(i);
		}
	}

	if(colors_list.empty()) //некуда перенести занятие
	{
		return false;
	}
	int new_color = colors_list[rand() % colors_list.size()];
#if 0
	if((new_color < 0)
		|| (new_color >= m_Graph.Colors().size()))
	{
		THROW("Color index impossble");
	}
#endif
	if(m_Graph.Items()[lesson_index].m)
	{
		++ m_MultiAudCount[m_Coloring[lesson_index].color];
		-- m_MultiAudCount[new_color];
	}
	else
	{
		++ m_SingleAudCount[m_Coloring[lesson_index].color];
		-- m_SingleAudCount[new_color];
	}
	m_Coloring[lesson_index].color = new_color;
	m_Fitness = FitnessCalculate();
	return true;
}

bool Individual::Crossover(const Individual& ind)
{
	bool result = false;
	std::vector<GraphForTime::ItemColoring> coloring(m_Graph.Items().size(), GraphForTime::ItemColoring(-1, -1, false));
	for(size_t i = 0; i < m_Graph.Items().size(); ++ i)
	{
		if(m_Coloring[i].color == ind.m_Coloring[i].color
			&& (m_Coloring[i].color > 0)) // если равны и закрашены
		{
			coloring[i].color = m_Coloring[i].color;
		}
		else
		{
			result = true;
		}
	}
	if(result) // если есть различия или незакрашенные вершины
	{
		m_Coloring = m_Graph.Coloring(coloring, &m_MultiAudCount, &m_SingleAudCount);
		m_Fitness = FitnessCalculate();
	}
	return result;
}

long Individual::FitnessCalculate() const
{
	long result = 0;
	for(size_t i = 0; i < m_Graph.Items().size(); ++ i) // индекс занятия
	{
		if(m_Coloring[i].color > 0)
		{
			for(size_t j = 0; j < m_Graph.Items().size(); ++ j) //индекс другого занятия
			{
				if(m_Graph.LinksGroup()[i][j]
					&& (m_Coloring[j].color > 0)
					&& (m_Coloring[i].color < m_Coloring[j].color)
					&& (m_Graph.Items()[i].b == m_Graph.Items()[j].b)
					&& (m_Graph.Items()[i].lt > m_Graph.Items()[j].lt))
				{
					// если занятие i раньше j, по той же дисциплине, но тип занятия больше
					result += UNORDERED_TYPES;
				}
			}
		}
		else
		{
			result += UNCOLORED_ITEM;
		}
	}
	return result;
}

void Individual::CheckSelf() const
{
	if(m_Coloring.size() != m_Graph.Items().size())
	{
		THROW("Different sizes items and vertex");
	}
	for(size_t i = 0; i < m_Coloring.size(); ++ i)
	{
		if((m_Coloring[i].a != -1)
			|| (m_Coloring[i].color < 0)
			|| (m_Coloring[i].color >= m_Graph.Colors().size()))
		{
			THROW("Invalid item");
		}
		for(size_t j = 0; j < m_Coloring.size(); ++ i)
		{
			if(m_Graph.Links()[i][j]
				&& (m_Coloring[i].color == m_Coloring[j].color))
			{
				THROW("Same color for neighbourhood vertex");
			}
		}
	}
}

