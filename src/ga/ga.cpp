#include "ga.h"
#include "../db/models.h"
#include "fitness.h"

// База расписания (занятие в количестве проводимых часов для размещения времени и аудитории
struct TableBase
{
	const Task &task; //занятие

};

long inline db2index(const std::vector<size_t>& ids, long db_index)
{
	return std::lower_bound(ids.begin(), ids.end(), db_index) - ids.begin();
}

GA::GA(DB::DataBase &db)
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
/*
	// получаем список G*T
	
	std::vector<ItemGT> gt_base;
	ORM::Tuple<long, long> scheme_gt;
	Glib::RefPtr<ORM::Table> table_gt = ORM::Table::create(scheme_gt);
	db.GetGTList(table_gt);
*/
	// получаем соответствие между занятиями и аудиториями
	less_aud.resize(ids_a.size() * ids_l.size(), false); //адресация [l*ids_a.size() + a]
	{
		ORM::Tuple<long, long> scheme_la;
		Glib::RefPtr<ORM::Table> table_la = ORM::Table::create(scheme_la);
		db.GetALList(table_la);

		for(Gtk::TreeIter it = table_la->children().begin(); it != table_la->children().end(); it ++)
		{
			less_aud[db2index(ids_l, it->get_value(scheme_la.f1)) * ids_a.size() + db2index(ids_a, it->get_value(scheme_la.f2))] = true;
		}
	}
	
	// получаем массив занятий Task
	{
		Glib::RefPtr<ORM::Table> table_tasks = ORM::Table::create(DB::g_ModelLessonRecords);
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
	ADH::SetSizes(A, D, H);
	Individual::SetMultiAud(multi_aud);

	// генерируем случайную популяцию
	const size_t POPULATION_SIZE = 20;
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

	const size_t MAX_STABLE_LOOPS = 100;
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
		}
		else
		{
			loops --;
		}
		gen ++;
	}
}

bool GA::Loop(std::vector<Individual> *population)
{
	std::vector<Fitness> fitnesses;
	for(std::vector<Individual>::iterator it = population->begin(); it != population->end(); it ++)
	{
		size_t errors;
		size_t quality;
		
		// ошибки соответсвия аудиторий и занятий
		
	}
	return false;
}

