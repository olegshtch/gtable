#ifndef _ORM_CASE_H_
#define _ORM_CASE_H_

#include "expr.h"

namespace ORM
{
	template<class T> class Case : public Expr<T>
	{
	public:
		template<class T1> Case(const Expr<T1>& when, const Expr<T>& then)
		{
			Expr<T>::m_Expr = "CASE WHEN " + when.m_Expr + " THEN " + then.m_Expr;
		}
		template<class T1> Case& When(const Expr<T1>& when, const Expr<T>& then)
		{
			Expr<T>::m_Expr += " WHEN " + when.m_Expr + " THEN " + then.m_Expr;
			return *this;
		}
		const Case& Else(const Expr<T>& els)
		{
			Expr<T>::m_Expr += " ELSE " + els.m_Expr + " END";
			return *this;
		}
	private:
	};
}

#endif

