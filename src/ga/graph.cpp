#include <fstream>
#include <algorithm>
#include "graph.h"
#include "../db/db.h"

Glib::ustring get_item_name(DB::DataBase &db, const GraphForTime::ItemTGBL& item)
{
	Glib::ustring res = db.GetTextById(DB::g_ModelTeachers, ORM::Expr<Glib::ustring>(DB::g_ModelTeachers.secondname) + " " + ORM::substr(DB::g_ModelTeachers.firstname, 1, 1) + ". " + ORM::substr(DB::g_ModelTeachers.thirdname, 1, 1) + ".", item.t);
	res += "\\n";
	res += db.GetTextById(DB::g_ModelGroups, DB::g_ModelGroups.name, item.g);
	res += "\\n";
	res += db.GetTextById(DB::g_ModelBranch, DB::g_ModelBranch.abbr, item.b);
	res += "\\\\";
	res += db.GetTextById(DB::g_ModelLessonType, DB::g_ModelLessonType.abbr, item.l);
	return res;
}

GraphForTime::GraphForTime()
{
	std::vector<ItemColor> colors;

	DB::DataBase &db = DB::DataBase::Instance();
	//getting all items
	{
		ORM::Scheme items_scheme;
		ORM::Field<long int> t("t");
		ORM::Field<long int> g("g");
		ORM::Field<long int> b("b");
		ORM::Field<long int> l("l");
		ORM::Field<long int> h("h");
		ORM::Field<bool> m("m");
		items_scheme.add(t);
		items_scheme.add(g);
		items_scheme.add(b);
		items_scheme.add(l);
		items_scheme.add(h);
		items_scheme.add(m);
		Glib::RefPtr<ORM::Data> data = ORM::Data::create(items_scheme);

		db.ListTGBLH(data);

		for(Gtk::TreeIter iter = data->children().begin(); iter != data->children().end(); ++ iter)
		{
			for(long int hours = iter->get_value(h); hours > 0; -- hours)
			{
				m_Items.push_back(ItemTGBL(iter->get_value(t)
					, iter->get_value(g)
					, iter->get_value(b)
					, iter->get_value(l)
					, iter->get_value(m)));
			}
		}

		// getting auditoriums list
		const size_t am_count = db.CountAuditoriums(true);
		const size_t as_count = db.CountAuditoriums(false);

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
				colors.push_back(ItemColor(day->get_value(id), hour->get_value(id), am_count, as_count));
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
			bool lection = m_Items[row].m && m_Items[col].m && (m_Items[row].l == m_Items[col].l) && (m_Items[row].b == m_Items[col].b);
			bool link = (m_Items[row].g == m_Items[col].g) || ((! lection) && (m_Items[row].t == m_Items[col].t));
			m_Links[row][col] = link;
			m_Links[col][row] = link;
		}
	}

	std::vector<std::pair<int, bool> > coloring(m_Items.size(), std::make_pair(-1, false)); // цвет, метка(соседствует с ракрашенной текущим цветом)

	// coloring time
	size_t color_index = 0;
	{
		std::vector<std::pair<size_t, size_t> > link_count; // список пар сумма и индекс
		for(row = 0; row < m_Items.size(); ++ row)
		{
			size_t sum = 0;
			if(coloring[row].first != -1)
			{
				for(size_t col = 0; col < m_Items.size(); ++ col)
				{
					if((coloring[col].first != -1) && m_Links[row][col]) // исключая закрашенные вершины
					{
						++ sum;
					}
				}
			}
			link_count.push_back(std::make_pair(sum, row));
		}
		std::sort(link_count.begin(), link_count.end());

		for(size_t i = 0; i < link_count.size(); ++ i)
		{
			const size_t index = link_count[i].second;
			if((! coloring[index].second) && (coloring[index].first != -1))
			{
				// не отмечена и не закрашена
				if(m_Items[index].m) // многопоточное занятие
				{
					if(colors[color_index].am > 0)
					{
						-- colors[color_index].am;
						coloring[index].first = color_index;
					}
				}
				else // однопоточное занятие
				{
					if(colors[color_index].as > 0)
					{
						-- colors[color_index].as;
						coloring[index].first = color_index;
					}
				}
				
			}
		}
	}

	//Dump();
}

void GraphForTime::Dump()
{
	DB::DataBase &db = DB::DataBase::Instance();
	std::ofstream f("dump.dot");
	f << "graph G{\n";
	// nodes
	for(size_t item = 0; item < m_Items.size(); ++ item)
	{
		f << "N" << item << " [label=\"" << get_item_name(db, m_Items[item]) << "\"]\n";
	}
	f << "\n";
	for(size_t row = 0; row < m_Items.size(); ++ row)
	{
		for(size_t col = row + 1; col < m_Items.size(); ++ col)
		{
			if(m_Links[col][row])
			{
				f << "N" << col << "--N" << row << "\n";
			}
		}
	}
	f << "}" << std::endl;
}

