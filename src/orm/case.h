#ifndef _ORM_CASE_H_
#define _ORM_CASE_H_

#include "expr.h"

namespace ORM
{
	template<class T> class Case : public Expr<T>
	{
	public:
		Case(const ExprBase& when, const Expr<T>& then)
		{
			this->GetExpr() = "CASE WHEN " + when.GetQuery() + " THEN " + then.GetQuery();
		}
		Case& When(const ExprBase& when, const Expr<T>& then)
		{
			this->GetExpr() += " WHEN " + when.GetQuery() + " THEN " + then.GetQuery();
			return *this;
		}
		const Case& Else(const Expr<T>& els)
		{
			this->GetExpr() += " ELSE " + els.GetQuery() + " END";
			return *this;
		}
	private:
	};
}

#endif

