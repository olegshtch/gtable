#ifndef _GA_GRAPH_H_
#define _GA_GRAPH_H_

#include <vector>

class GraphForTime
{
public:
	struct ItemTSM
	{
		long int t; // преподаватель
		long int s; // поток
		bool m; // многопоточность
		long int a; // аудитория

		ItemTSM(long int t_, long int s_, bool m_)
			:t(t_),
			s(s_),
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

	GraphForTime();
	~GraphForTime()
	{
	}

	void Dump();
private:
	std::vector<ItemTSM> m_Items;
	std::vector<std::vector<char> > m_Links;
};

#endif

