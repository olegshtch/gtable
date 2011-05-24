#include <fstream>
#include <algorithm>
#include "graph.h"
#include "../db/db.h"
#include "logbuf.h"

GraphForTime::GraphForTime()
{
	DB::DataBase &db = DB::DataBase::Instance();
	{
		//getting all items
		ORM::Scheme items_scheme;
		ORM::Field<long int> t("t");
		ORM::Field<long int> l("l");
		ORM::Field<long int> lt("lt");
		ORM::Field<long int> b("b");
		ORM::Field<long int> h("h");
		ORM::Field<bool> m("m");
		items_scheme.add(t);
		items_scheme.add(l);
		items_scheme.add(lt);
		items_scheme.add(b);
		items_scheme.add(h);
		items_scheme.add(m);
		
		Glib::RefPtr<ORM::Data> data = ORM::Data::create(items_scheme);

		db.ListTLBHM(data);

		for(Gtk::TreeIter iter = data->children().begin(); iter != data->children().end(); ++ iter)
		{
			for(long int hours = iter->get_value(h); hours > 0; -- hours)
			{
				m_Items.push_back(ItemTLBM(iter->get_value(t)
					, iter->get_value(l)
					, iter->get_value(lt)
					, iter->get_value(b)
					, iter->get_value(m)));
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
				m_Colors.push_back(ItemColor(day->get_value(id), hour->get_value(id), am_count, as_count));
			}
		}

		//getting auditoriums
		Glib::RefPtr<ORM::Data> list_aud = ORM::Data::create(DB::g_ModelAuditoriums);
		db.ListEntityOrdered(DB::g_ModelAuditoriums, list_aud, DB::g_ModelAuditoriums.fId);
		for(Gtk::TreeIter aud = list_aud->children().begin(); aud != list_aud->children().end(); ++ aud)
		{
			if(aud->get_value(DB::g_ModelAuditoriums.multithread))
			{
				m_MultiAuds.push_back(aud->get_value(DB::g_ModelAuditoriums.fId));
			}
			else
			{
				m_SingleAuds.push_back(aud->get_value(DB::g_ModelAuditoriums.fId));
			}
		}

		// fill item holydays
		for(size_t i = 0; i < m_Items.size(); ++ i)
		{
			for(size_t c = 0; c < m_Colors.size(); ++ c)
			{
				if(db.GetLessonHolydays(m_Items[i].l, m_Colors[c].d, m_Colors[c].h))
				{
					m_Holydays.insert(std::make_pair(i, c));
				}
			}
		}
	}
	
	m_Links.resize(m_Items.size());
	m_LinksGroup.resize(m_Items.size());
	size_t row;
	for(row = 0; row < m_Items.size(); ++ row)
	{
		m_Links[row].resize(m_Items.size());
		m_LinksGroup[row].resize(m_Items.size());
	}
	for(row = 0; row < m_Items.size(); ++ row)
	{
		for(size_t col = row + 1; col < m_Items.size(); ++ col)
		{
			const bool link_group = db.InterseptGroups(m_Items[row].l, m_Items[col].l);
			const bool link = (m_Items[row].t == m_Items[col].t) || (m_Items[row].l == m_Items[col].l) || link_group;
			/*if(link)
			{
				std::cout << "link row=" << row << " col=" << col << std::endl;
			}*/
			m_Links[row][col] = link;
			m_Links[col][row] = link;
			m_LinksGroup[row][col] = link_group;
			m_LinksGroup[col][row] = link_group;
		}
	}

	// данные загружены
	
	//std::vector<GraphForTime::ItemColoring> coloring = Coloring();

	// генетические алгоритмы (std::vector<ItemColoring> - особь)
	
	//StoreTimetable(coloring);
}

std::vector<GraphForTime::ItemColoring> GraphForTime::Coloring(std::vector<size_t> *p_multi_aud_count, std::vector<size_t> *p_single_aud_count) const
{
	size_t row;
	std::vector<ItemColoring> coloring(m_Items.size(), ItemColoring(-1, -1, false));

	std::vector<size_t> multi_aud_count(m_Colors.size(), 0);
	std::vector<size_t> single_aud_count(m_Colors.size(), 0);

	// coloring time
	size_t color_index = 0;
	do
	{
		multi_aud_count[color_index] = m_Colors[color_index].am;
		single_aud_count[color_index] = m_Colors[color_index].as;

		std::vector<std::pair<size_t, size_t> > link_count; // список пар сумма и индекс
		for(row = 0; row < m_Items.size(); ++ row)
		{
			size_t sum = 0;
			if(coloring[row].color == -1)
			{
				// незакрашена
				for(size_t col = 0; col < m_Items.size(); ++ col)
				{
					if((coloring[col].color == -1) && m_Links[row][col]) // исключая закрашенные вершины
					{
						sum += 1;
					}
				}
				// подсчёт связей для выходных (todo: или только для текущего цвета)
				for(size_t c = 0; c < m_Colors.size(); ++ c)
				{
					if(m_Holydays.count(std::make_pair(row, c)))
					{
						sum += 1;
					}
				}
			}
			link_count.push_back(std::make_pair(sum, row));
		}
		std::sort(link_count.begin(), link_count.end(), std::greater<std::pair<size_t, size_t> >());

		// отсортировано в порядке убывания

		for(size_t i = 0; i < link_count.size(); ++ i)
		{
			//std::cout << link_count[i].first << "," << link_count[i].second << std::endl;
			const size_t index = link_count[i].second; // индекс вершины
			if((! coloring[index].mark)
				&& (coloring[index].color == -1)
				&& (! m_Holydays.count(std::make_pair(index, color_index))))
			{
				// не отмечена и не закрашена
				if(m_Items[index].m) // многопоточное занятие
				{
					if(multi_aud_count[color_index] > 0)
					{
						-- multi_aud_count[color_index];
						coloring[index].color = color_index;
					}
				}
				else // однопоточное занятие
				{
					if(single_aud_count[color_index] > 0)
					{
						-- single_aud_count[color_index];
						coloring[index].color = color_index;
					}
				}
				if(coloring[index].color == color_index)
				{
					// если закрасили, отметим соседние
					for(size_t j = 0; j < m_Items.size(); ++ j)
					{
						if(m_Links[index][j])
						{
							coloring[j].mark = true;
						}
					}
				}
			}
		}
		// убираем отметки
		for(size_t j = 0; j < m_Items.size(); ++ j)
		{
			coloring[j].mark = false;
		}

		color_index ++;

	} // конец шага закраски
	while(color_index < m_Colors.size());

	if(p_multi_aud_count)
	{
		(*p_multi_aud_count) = multi_aud_count;
	}
	if(p_single_aud_count)
	{
		(*p_single_aud_count) = single_aud_count;
	}

	return coloring;
}

std::vector<GraphForTime::ItemColoring> GraphForTime::Coloring(const std::vector<GraphForTime::ItemColoring>& coloring_, std::vector<size_t> *p_multi_aud_count, std::vector<size_t> *p_single_aud_count) const
{
	size_t row;
	std::vector<ItemColoring> coloring(coloring_);

	std::vector<size_t> multi_aud_count(m_Colors.size(), 0);
	std::vector<size_t> single_aud_count(m_Colors.size(), 0);

	// coloring time
	size_t color_index = 0;
	do
	{
		multi_aud_count[color_index] = m_Colors[color_index].am;
		single_aud_count[color_index] = m_Colors[color_index].as;

		//проверка на текущий цвет и расстановка меток
		std::vector<std::pair<size_t, size_t> > link_count; // список пар сумма и индекс
		for(row = 0; row < m_Items.size(); ++ row)
		{
			size_t sum = 0;
			if(coloring[row].color == -1)
			{
				// незакрашена
				for(size_t col = 0; col < m_Items.size(); ++ col)
				{
					if((coloring[col].color == -1) && m_Links[row][col]) // исключая закрашенные вершины
					{
						sum += 1;
					}
				}
				// подсчёт связей для выходных (todo: или только для текущего цвета)
				for(size_t c = 0; c < m_Colors.size(); ++ c)
				{
					sum += m_Holydays.count(std::make_pair(row, c));
				}
			}
			else if(coloring[row].color == color_index)
			{
				for(size_t col = 0; col < m_Items.size(); ++ col)
				{
					if(m_Links[row][col])
					{
						coloring[col].mark = true; // помечаем связанные
					}
				}

				// учитываем загрузку аудиторий
				if(m_Items[row].m)
				{
					-- multi_aud_count[color_index];
				}
				else
				{
					-- single_aud_count[color_index];
				}
			}
			link_count.push_back(std::make_pair(sum, row));
		}
		std::sort(link_count.begin(), link_count.end(), std::greater<std::pair<size_t, size_t> >());

		// отсортировано в порядке убывания

		for(size_t i = 0; i < link_count.size(); ++ i)
		{
			//std::cout << link_count[i].first << "," << link_count[i].second << std::endl;
			const size_t index = link_count[i].second; // индекс вершины
			if((! coloring[index].mark)
				&& (coloring[index].color == -1)
				&& (! m_Holydays.count(std::make_pair(index, color_index))))
			{
				// не отмечена и не закрашена
				if(m_Items[index].m) // многопоточное занятие
				{
					if(multi_aud_count[color_index] > 0)
					{
						-- multi_aud_count[color_index];
						coloring[index].color = color_index;
					}
				}
				else // однопоточное занятие
				{
					if(single_aud_count[color_index] > 0)
					{
						-- single_aud_count[color_index];
						coloring[index].color = color_index;
					}
				}
				if(coloring[index].color == color_index)
				{
					// если закрасили, отметим соседние
					for(size_t j = 0; j < m_Items.size(); ++ j)
					{
						if(m_Links[index][j])
						{
							coloring[j].mark = true;
						}
					}
				}
			}
		}
		// убираем отметки
		for(size_t j = 0; j < m_Items.size(); ++ j)
		{
			coloring[j].mark = false;
		}

		color_index ++;

	} // конец шага закраски
	while(color_index < m_Colors.size());

	if(p_multi_aud_count)
	{
		(*p_multi_aud_count) = multi_aud_count;
	}
	if(p_single_aud_count)
	{
		(*p_single_aud_count) = single_aud_count;
	}

	return coloring;
}

void GraphForTime::StoreTimetable(const std::vector<GraphForTime::ItemColoring>& coloring) const
{
	DB::DataBase &db = DB::DataBase::Instance();
	// сохранить расписание
	db.CleanTimeTable();
	for(size_t i = 0; i < m_Items.size() ; ++ i)
	{
		if(coloring[i].color != -1)
		{
			if(m_Items[i].m)
			{
				// поиск свободной многопоточной аудитории
				for(size_t a = 0; a < m_MultiAuds.size(); a ++)
				{
					if(db.SetLessonIntoTimetable(m_Items[i].l, m_MultiAuds[a], m_Colors[coloring[i].color].d, m_Colors[coloring[i].color].h))
					{
						break;
					}
				}
			}
			else
			{
				// поиск свободной однопоточной аудитории
				for(size_t a = 0; a < m_SingleAuds.size(); a ++)
				{
					if(db.SetLessonIntoTimetable(m_Items[i].l, m_SingleAuds[a], m_Colors[coloring[i].color].d, m_Colors[coloring[i].color].h))
					{
						break;
					}
				}

			}
		}
		else
		{
			std::cout << "Timetable doesn't fill" << std::endl;
		}
	}
}

