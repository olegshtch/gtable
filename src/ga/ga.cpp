#include "ga.h"
#include "../db/models.h"
#include "individual.h"

// Занятие
struct Task
{
	long g; //группа
	long t; //преподаватель
	long l; //занятие

	Task(long g_, long t_, long l_)
		:g(g_), t(t_), l(l_)
	{
	}
};

// размещаемое условие
struct Place
{
	long a; // аудитория
	long d; // дни
	long h; // час
	Place(long a_, long d_, long h_)
		:a(a_), d(d_), h(h_)
	{
	}
};

void GA::Solve(DB::DataBase &db)
{
	Glib::RefPtr<ORM::Table> base_table = ORM::Table::create(DB::g_ModelLessonRecords);
	db.ListLessonRecords(base_table);
	
	std::vector<Task> base;
	//заполняем список занятий
	for(Gtk::TreeIter it = base_table->children().begin(); it !=  base_table->children().end(); it ++)
	{
		for(long i = 0; i < it->get_value(DB::g_ModelLessonRecords.hours); i ++)
		{
			base.push_back(Task(
				it->get_value(DB::g_ModelLessonRecords.g_id),
				it->get_value(DB::g_ModelLessonRecords.t_id),
				it->get_value(DB::g_ModelLessonRecords.l_id)));
		}
	}
	std::cout << "Size of g*t*l = " << base.size() << std::endl;
	std::cout << "Size of a*d*h = " << db.GetEntitiesCount(DB::g_Auditoriums) * db.GetEntitiesCount(DB::g_Days) * db.GetEntitiesCount(DB::g_Hours) << std::endl;
}

