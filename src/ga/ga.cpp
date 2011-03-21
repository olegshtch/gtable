#include <fstream>
#include "ga.h"
#include "../db/models.h"
#include "fitness.h"
#include "individual.h"

// коэффициенты качества
const size_t GROUP_WINDOW = 5; //"окна" в расписании групп
const size_t TEACHER_DAY = 10; //рабочие дни у преподавателей

const size_t POPULATION_SIZE = 75;
const size_t MAX_STABLE_LOOPS = 200;

size_t GA::A, GA::D, GA::H, GA::G, GA::T, GA::L;

// База расписания (занятие в количестве проводимых часов для размещения времени и аудитории
/*struct TableBase
{
	const Task &task; //занятие
};*/

long inline db2index(const std::vector<size_t>& ids, long db_index)
{
	return std::lower_bound(ids.begin(), ids.end(), db_index) - ids.begin();
}

GA::GA()
{
}

bool GA::Loop(std::vector<Individual> *population)
{
	std::vector<Fitness> fitnesses;
	bool changed = false;
	for(std::vector<Individual>::iterator it = population->begin(); it != population->end(); it ++)
	{
		size_t errors = 0;
		size_t quality = 0;

		size_t i;
		
		for(i = 0; i < table_base.size(); i ++)
		{
			// ошибки соответствия аудиторий и занятий
			const long l = table_base[i]->l;
			const long a = ADH(it->GetTable()[i]).a;
			if(! less_aud[l * A + a])
			{
				errors ++;
			}

			// ошибки соответствия занятий многих групп
			if(multi_aud[a] && (it->GetBusy()[it->GetTable()[i]] > 1))
			{
				for(size_t j = i + 1; j < table_base.size(); j ++)
				{
					if(it->GetTable()[i] == it->GetTable()[j])
					{
						if((table_base[i]->t != table_base[j]->t) || (table_base[i]->l != table_base[j]->l))
						{
							errors ++;
						}
					}
				}
			}
		}

		// занятость групп
		i = 0;
		for(size_t g = 0; g < G; g ++)
		{
			std::vector<bool> busy_time(D * H, false); // адресация [d + D * h]
			while((i < table_base.size()) && (table_base[i]->g == g))
			{
				const long d = ADH(it->GetTable()[i]).d;
				const long h = ADH(it->GetTable()[i]).h;

				if(busy_time[d + D * h])
				{
					errors ++;
				}
				else
				{
					busy_time[d + D * h] = true;
				}
				
				i++;
			}

			//расчёт "окон" в расписании
			for(size_t d = 0; d < D; d ++)
			{
				bool study = false;
				size_t moves = 0;
				for(size_t h = 0; h < H; h ++)
				{
					if(busy_time[d + D * h] != study)
					{
						moves ++;
						study = busy_time[d + D * h];
					}
				}
				if(study)
				{
					moves ++;
				}
				if(moves > 2)
				{
					// в расписании есть (moves - 2) / 2 окон
					quality += GROUP_WINDOW * ((moves - 2) >> 1);
				}
			}

		}

		// занятость преподавателей
		i = 0;
		for(size_t t = 0; t < T; t ++)
		{
			std::vector<long> busy_time(D * H, -1); // адресация [d + D * h], -1 нет занятий, (l * A + l) - занятие
			for(i = 0; i < table_base.size(); i ++)
			{
				if(table_base[i]->t == t)
				{
					const long d = ADH(it->GetTable()[i]).d;
					const long h = ADH(it->GetTable()[i]).h;
					const long a = ADH(it->GetTable()[i]).a;

					if(busy_time[d + D * h] == -1)
					{
						busy_time[d + D * h] = table_base[i]->l * A + a;
					}
					else // уже есть занятие
					{
						if(multi_aud[a])
						{
							if(busy_time[d + D * h] != (table_base[i]->l * A + a))
							{
								errors ++;
							}
						}
						else
						{
							errors ++;
						}
					}
				}
			}

			// оценка занятости
			for(size_t d = 0; d < D; d ++)
			{
				bool study = false;
				for(size_t h = 0; h < H; h ++)
				{
					if(busy_time[d + D * h] != -1)
					{
						study = true;
						break;
					}
				}
				if(study)
				{
					quality += TEACHER_DAY;
				}
			}
		}

		//std::cout << "error: " << errors << " quality: " << quality << std::endl;
		fitnesses.push_back(Fitness(it, errors, quality));
	}

	std::vector<Fitness>::iterator elite = std::min_element(fitnesses.begin(), fitnesses.end());
	if(&elite->GetIndividual() == &population->at(0))
	{
		// лучшая особь не изменилась
		changed = false;
	}
	else
	{
		std::cout << "Fitness errors = " << elite->errors << " quality = " << elite->quality << std::endl;
		changed = true;
	}

	std::vector<Individual> new_population;
	new_population.push_back(elite->GetIndividual());

	// селекция
	std::vector<std::vector<Individual>::const_iterator> parent_pool;
	for(size_t i = 0; i < POPULATION_SIZE - 1; i ++)
	{
		// турнирная селекция
		size_t i1 = rand() % POPULATION_SIZE;
		size_t i2 = rand() % POPULATION_SIZE;
		parent_pool.push_back(std::min(fitnesses[i1], fitnesses[i2]).GetIter());
	}

	// создание новой популяции
	while(new_population.size() < POPULATION_SIZE)
	{
		if(rand() % 2)
		{
			// мутация
			size_t i1 = rand() % parent_pool.size();
			new_population.push_back(*parent_pool[i1]);
			new_population[new_population.size() - 1].Mutation();
		}
		else
		{
			// кроссовер
			size_t i1 = rand() % parent_pool.size();
			size_t i2 = rand() % parent_pool.size();
			new_population.push_back(*parent_pool[i1]);
			new_population[new_population.size() - 1].Crossover(*parent_pool[i2]);
		}
	}

	(*population) = new_population;

	return changed;
}

