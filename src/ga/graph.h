#ifndef _GA_GRAPH_H_
#define _GA_GRAPH_H_

#include <vector>
#include <set>

class GraphForTime
{
public:
	struct ItemTLBM
	{
		long int t; // преподаватель
		long int l; // занятие
		long int lt; // тип занятия
		long int b; // дисциплина
		bool m; // многопоточность

		ItemTLBM(long int t_, long int l_, long int lt_, long int b_, bool m_)
			:t(t_),
			l(l_),
			lt(lt_),
			b(b_),
			m(m_) 
		{
		}
	};

	struct ItemColor
	{
		long int d;
		long int h;
		size_t am; // доступных многопоточных аудиторий
		size_t as; // доступных однопоточных аудиторий

		ItemColor(long int d_, long int h_, size_t am_, size_t as_)
			:d(d_),
			h(h_),
			am(am_),
			as(as_)
		{
		}
	};

	struct ItemColoring
	{
		long int a; // аудитория
		int color; //цвет
		bool mark; //метка

		ItemColoring(long int a_, int color_, bool mark_)
			:a(a_),
			color(color_),
			mark(mark_)
		{
		}
	};

	GraphForTime();
	~GraphForTime()
	{
	}

	// раскраска с нуля
	std::vector<GraphForTime::ItemColoring> Coloring(std::vector<size_t> *p_multi_aud_count, std::vector<size_t> *p_single_aud_count) const;

	// дораскраска
	std::vector<GraphForTime::ItemColoring> Coloring(const std::vector<GraphForTime::ItemColoring>& coloring_, std::vector<size_t> *p_multi_aud_count, std::vector<size_t> *p_single_aud_count) const;

	void StoreTimetable(const std::vector<GraphForTime::ItemColoring>& coloring) const;

	const std::vector<ItemTLBM>& Items() const
	{
		return m_Items;
	}
	const std::vector<std::vector<char> >& Links() const
	{
		return m_Links;
	}
	const std::vector<ItemColor>& Colors() const
	{
		return m_Colors;
	}
	const std::set<std::pair<size_t, size_t> >& Holydays() const
	{
		return m_Holydays;
	}
private:
	std::vector<ItemTLBM> m_Items;
	std::vector<std::vector<char> > m_Links;
	std::vector<ItemColor> m_Colors;
	std::vector<long int> m_MultiAuds;
	std::vector<long int> m_SingleAuds;
	std::set<std::pair<size_t, size_t> > m_Holydays; //item, color;
};

#endif

