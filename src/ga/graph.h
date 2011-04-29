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
	};

	GraphForTime();
	~GraphForTime()
	{
	}
private:
	std::vector<ItemTGBL> m_Items;
};

#endif

