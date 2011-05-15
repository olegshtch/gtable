#ifndef _GA_GRAPH_H_
#define _GA_GRAPH_H_

#include <vector>
#include <glibmm/dispatcher.h>

class GraphForTime
{
public:
	struct ItemTLM
	{
		long int t; // преподаватель
		long int l; // занятие
		bool m; // многопоточность
		long int a; // аудитория

		ItemTLM(long int t_, long int l_, bool m_)
			:t(t_),
			l(l_),
			m(m_),
			a(-1)
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

	GraphForTime(Glib::Dispatcher &dispatcher);
	~GraphForTime()
	{
	}

	void Dump();
private:
	std::vector<ItemTLM> m_Items;
	std::vector<std::vector<char> > m_Links;
};

#endif

