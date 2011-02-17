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

GA::GA(DB::DataBase &db_)
	:db(db_)
{
	// создаём список идентификаторов БД:
	
	
	ids_a.resize(db.GetEntitiesCount(DB::g_Auditoriums));
	db.GetEntitiesIDs(DB::g_Auditoriums, &ids_a);
	ids_d.resize(db.GetEntitiesCount(DB::g_Days));
	db.GetEntitiesIDs(DB::g_Days, &ids_d);
	ids_h.resize(db.GetEntitiesCount(DB::g_Hours));
	db.GetEntitiesIDs(DB::g_Hours, &ids_h);

	A = ids_a.size();
	D = ids_d.size();
	H = ids_h.size();

	ids_g.resize(db.GetEntitiesCount(DB::g_Groups));
	db.GetEntitiesIDs(DB::g_Groups, &ids_g);
	ids_t.resize(db.GetEntitiesCount(DB::g_Teachers));
	db.GetEntitiesIDs(DB::g_Teachers, &ids_t);
	ids_l.resize(db.GetEntitiesCount(DB::g_Lessons));
	db.GetEntitiesIDs(DB::g_Lessons, &ids_l);

	G = ids_g.size();
	T = ids_t.size();
	L = ids_l.size();

/*
	// получаем список G*T
	
	std::vector<ItemGT> gt_base;
	ORM::Tuple<long, long> scheme_gt;
	Glib::RefPtr<ORM::Data> table_gt = ORM::Data::create(scheme_gt);
	db.GetGTList(table_gt);
*/
	// получаем соответствие между занятиями и аудиториями
	less_aud.resize(ids_a.size() * ids_l.size(), false); //адресация [l*ids_a.size() + a]
	{
		ORM::Tuple<long, long> scheme_la;
		Glib::RefPtr<ORM::Data> table_la = ORM::Data::create(scheme_la);
		db.GetALList(table_la);

		for(Gtk::TreeIter it = table_la->children().begin(); it != table_la->children().end(); it ++)
		{
			less_aud[db2index(ids_l, it->get_value(scheme_la.f1)) * A + db2index(ids_a, it->get_value(scheme_la.f2))] = true;
		}
	}
	
	// получаем массив занятий Task
	{
		Glib::RefPtr<ORM::Data> table_tasks = ORM::Data::create(DB::g_ModelLessonRecords);
		db.ListLessonRecords(table_tasks);

		for(Gtk::TreeIter it = table_tasks->children().begin(); it != table_tasks->children().end(); it ++)
		{
			tasks.push_back(Task(
				db2index(ids_g, it->get_value(DB::g_ModelLessonRecords.g_id)),
				db2index(ids_t, it->get_value(DB::g_ModelLessonRecords.t_id)),
				db2index(ids_l, it->get_value(DB::g_ModelLessonRecords.l_id)),
				it->get_value(DB::g_ModelLessonRecords.hours)));
		}
	}

	for(std::vector<Task>::const_iterator it = tasks.begin(); it != tasks.end(); it ++)
	{
		for(size_t i = 0; i < it->hours; i ++)
		{
			table_base.push_back(it);
		}
	}

	// получаем список многопоточных аудиторий
	multi_aud.resize(ids_a.size(), false);
	for(size_t i = 0; i < ids_a.size(); i ++)
	{
		multi_aud[i] = db.GetAudMultithr(DB::g_Auditoriums, ids_a[i]);
	}

	//Run(less_aud, table_base, multi_aud, ids_a.size(), ids_d.size(), ids_h.size());

}

void GA::Run()
{
	Individual::SetGA(this);

	// генерируем случайную популяцию
	std::vector<Individual> population;
	for(size_t i = 0; i < POPULATION_SIZE; i ++)
	{
		population.push_back(Individual(table_base.size(), A * D * H));
		for(size_t j = 0; j < table_base.size(); j ++)
		{
			long adh_index = -1;
			do
			{
				adh_index = rand() % (A * D * H);
			}
			while(! population[i].SetTable(j, adh_index));
		}
	}
	std::cout << "Generated" << std::endl;

	size_t loops = MAX_STABLE_LOOPS;
	size_t gen = 0;
	//цикл ГА
	while(loops)
	{
		bool best_changed = false;
		// next gen
		best_changed = Loop(&population);
		if(best_changed)
		{
			loops = MAX_STABLE_LOOPS;
			std::cout << "Found new best" << std::endl;
		}
		else
		{
			loops --;
		}
		gen ++;
		std::cout << "Generation: " << gen << std::endl;
	}
	std::ofstream f("answer.txt");
	population[0].Output(f);
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

