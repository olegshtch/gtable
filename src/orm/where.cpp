#include "subquery.h"

ORM::NotIn::NotIn(const ExprBase& expr, const Subquery& subquery)
	:WhereBase(expr.GetQuery() + " NOT IN (" + subquery.GetQuery() + ")")
{
}

ORM::In::In(const ExprBase& expr, const Subquery& subquery)
	:WhereBase(expr.GetQuery() + " IN (" + subquery.GetQuery() + ")")
{
}

