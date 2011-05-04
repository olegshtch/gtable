#ifndef _GA_GRAPH_H_
#define _GA_GRAPH_H_

#include <vector>

class GraphForTime
{
public:
	struct ItemTGBL
	{
		long int t;
		long int g;
		long int b;
		long int l;
		bool m;

		ItemTGBL(long int t_, long int g_, long int b_, long int l_, bool m_)
			:t(t_),
			g(g_),
			b(b_),
			l(l_),
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

	GraphForTime();
	~GraphForTime()
	{
	}

	void Dump();
private:
	std::vector<ItemTGBL> m_Items;
	std::vector<std::vector<bool> > m_Links;
};

#endif

