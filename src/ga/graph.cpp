#include "graph.h"
#include "../db/db.h"

GraphForTime::GraphForTime()
{
	//getting all items
	{
		ORM::Scheme items_scheme;
		ORM::Field<long int> t("t");
		ORM::Field<long int> g("g");
		ORM::Field<long int> b("b");
		ORM::Field<long int> l("l");
		items_scheme.add(t);
		items_scheme.add(g);
		items_scheme.add(b);
		items_scheme.add(l);
	}
}

