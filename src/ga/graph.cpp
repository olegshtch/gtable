#include <fstream>
#include <algorithm>
#include "graph.h"
#include "../db/db.h"
#include "logbuf.h"

GraphForTime::GraphForTime()
{
	std::vector<ItemColor> colors;
	std::vector<long int> multi_aud;
	std::vector<long int> single_aud;
	std::set<std::pair<size_t, size_t> > item_holydays; //item, color;

	DB::DataBase &db = DB::DataBase::Instance();
	{
		//getting all items
		ORM::Scheme items_scheme;
		ORM::Field<long int> t("t");
		ORM::Field<long int> l("l");
		ORM::Field<long int> h("h");
		ORM::Field<bool> m("m");
		items_scheme.add(t);
		items_scheme.add(l);
		items_scheme.add(h);
		items_scheme.add(m);
		Glib::RefPtr<ORM::Data> data = ORM::Data::create(items_scheme);

		db.ListTLHM(data);

		for(Gtk::TreeIter iter = data->children().begin(); iter != data->children().end(); ++ iter)
		{
			for(long int hours = iter->get_value(h); hours > 0; -- hours)
			{
				m_Items.push_back(ItemTLM(iter->get_value(t)
					, iter->get_value(l)
					, iter->get_value(m)));
				std::cout << "t=" << iter->get_value(t)
					<< "l=" << iter->get_value(l)
					<< "m=" << iter->get_value(m) << std::endl;
			}
		}

		// getting colors
		ORM::Scheme scheme;
		ORM::Field<long int> id("");
		scheme.add(id);

		Glib::RefPtr<ORM::Data> list_hours = ORM::Data::create(scheme);
		db.ListIDs(DB::g_ModelHours, list_hours);
		Glib::RefPtr<ORM::Data> list_days = ORM::Data::create(scheme);
		db.ListIDs(DB::g_ModelDays, list_days);

		for(Gtk::TreeIter day = list_days->children().begin(); day != list_days->children().end(); ++ day)
		{
			for(Gtk::TreeIter hour = list_hours->children().begin(); hour != list_hours->children().end(); ++ hour)
			{
				//count non-holyday auditorium for day, hour
				const size_t am_count = db.CountAuditoriums(true, day->get_value(id), hour->get_value(id));
				const size_t as_count = db.CountAuditoriums(false, day->get_value(id), hour->get_value(id));
				colors.push_back(ItemColor(day->get_value(id), hour->get_value(id), am_count, as_count));
			}
		}

		//getting auditoriums
		Glib::RefPtr<ORM::Data> list_aud = ORM::Data::create(DB::g_ModelAuditoriums);
		db.ListEntity(DB::g_ModelAuditoriums, list_aud);
		for(Gtk::TreeIter aud = list_aud->children().begin(); aud != list_aud->children().end(); ++ aud)
		{
			if(aud->get_value(DB::g_ModelAuditoriums.multithread))
			{
				multi_aud.push_back(aud->get_value(DB::g_ModelAuditoriums.fId));
			}
			else
			{
				single_aud.push_back(aud->get_value(DB::g_ModelAuditoriums.fId));
			}
		}

		// fill item holydays
		for(size_t i = 0; i < m_Items.size(); ++ i)
		{
			for(size_t c = 0; c < colors.size(); ++ c)
			{
				if(db.GetLessonHolydays(m_Items[i].l, colors[c].d, colors[c].h))
				{
					item_holydays.insert(std::make_pair(i, c));
				}
			}
		}
	}
	
	m_Links.resize(m_Items.size());
	size_t row;
	for(row = 0; row < m_Items.size(); ++ row)
	{
		m_Links[row].resize(m_Items.size());
	}
	for(row = 0; row < m_Items.size(); ++ row)
	{
		for(size_t col = row + 1; col < m_Items.size(); ++ col)
		{
			const bool link = (m_Items[row].t == m_Items[col].t) || (m_Items[row].l == m_Items[col].l) || db.InterseptLessons(m_Items[row].l, m_Items[col].l);
			/*if(link)
			{
				std::cout << "link row=" << row << " col=" << col << std::endl;
			}*/
			m_Links[row][col] = link;
			m_Links[col][row] = link;
		}
	}

	std::vector<std::pair<int, bool> > coloring(m_Items.size(), std::make_pair(-1, false)); // цвет, метка(соседствует с ракрашенной текущим цветом)

	// coloring time
	size_t color_index = 0;
	do
	{
		std::cout << "Coloring step " << color_index << std::endl;
		std::vector<std::pair<size_t, size_t> > link_count; // список пар сумма и индекс
		for(row = 0; row < m_Items.size(); ++ row)
		{
			size_t sum = 0;
			if(coloring[row].first == -1)
			{
				// незакрашена
				for(size_t col = 0; col < m_Items.size(); ++ col)
				{
					if((coloring[col].first == -1) && m_Links[row][col]) // исключая закрашенные вершины
					{
						sum += 1;
					}
				}
				// подсчёт связей для выходных (todo: или только для текущего цвета)
				for(size_t c = 0; c < colors.size(); ++ c)
				{
					if(item_holydays.count(std::make_pair(row, c)))
					{
						sum += 1;
						std::cout << "holyday item=" << row << " color=" << c << std::endl;
					}
				}
			}
			link_count.push_back(std::make_pair(sum, row));
		}
		std::sort(link_count.begin(), link_count.end(), std::greater<std::pair<size_t, size_t> >());

		// отсортировано в порядке убывания

		for(size_t i = 0; i < link_count.size(); ++ i)
		{
			std::cout << link_count[i].first << "," << link_count[i].second << std::endl;
			const size_t index = link_count[i].second; // индекс вершины
			if((! coloring[index].second)
				&& (coloring[index].first == -1)
				&& (! item_holydays.count(std::make_pair(index, color_index))))
			{
				// не отмечена и не закрашена
				if(m_Items[index].m) // многопоточное занятие
				{
					if(colors[color_index].am > 0)
					{
						-- colors[color_index].am;
						coloring[index].first = color_index;
						std::cout << "Color " << index << " by " << color_index << std::endl;
					}
				}
				else // однопоточное занятие
				{
					if(colors[color_index].as > 0)
					{
						-- colors[color_index].as;
						coloring[index].first = color_index;
						std::cout << "Color " << index << " by " << color_index << std::endl;
					}
				}
				if(coloring[index].first == color_index)
				{
					// если закрасили, отметим соседние
					for(size_t j = 0; j < m_Items.size(); ++ j)
					{
						if(m_Links[index][j])
						{
							coloring[j].second = true;
						}
					}
				}
			}
		}
		// убираем отметки и распределяем аудитории
		for(size_t j = 0; j < m_Items.size(); ++ j)
		{
			coloring[j].second = false;
			if(coloring[j].first == color_index)
			{
				if(m_Items[j].m)
				{
					m_Items[j].a = multi_aud[colors[color_index].am];
					colors[color_index].am ++;
				}
				else
				{
					m_Items[j].a = single_aud[colors[color_index].as];
					colors[color_index].as ++;					
				}
			}
		}

		color_index ++;

	} // конец шага закраски
	while(color_index < colors.size());	
	
	// сохранить расписание
	db.CleanTimeTable();
	for(size_t i = 0; i < m_Items.size() ; ++ i)
	{
		if(coloring[i].first != -1)
		{
			db.SetLessonIntoTimetable(m_Items[i].l, m_Items[i].a, colors[coloring[i].first].d, colors[coloring[i].first].h);
		}
	}
}

