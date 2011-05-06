#ifndef _ORM_EXPR_H_
#define _ORM_EXPR_H_

#include "field.h"

namespace ORM
{
	class ExprBase
	{
	public:
		ExprBase(const ExprBase& expr)
			:m_Expr(expr.m_Expr)
		{
		}
		ExprBase(const FieldBase& field)
			:m_Expr(field.GetFieldName())
		{
		}
		Glib::ustring GetQuery() const
		{
			return m_Expr;
		}
		~ExprBase()
		{
		}
	protected:
		ExprBase(const Glib::ustring& expr)
			:m_Expr(expr)
		{
		}
		Glib::ustring m_Expr;
	};

	template<class T> class Expr : public ExprBase
	{
	public:
		Expr(const Field<T>& field)
			:ExprBase(field.GetFieldName())
		{
		}

/*		Expr(const Expr<T>& expr)
			:ExprBase(expr.m_Expr)
		{
		}*/

		Expr(const T& value)
			:ExprBase(Field<T>::ToString(value))
		{
		}

		~Expr()
		{
		}
	protected:
		Glib::ustring& GetExpr()
		{
			return this->m_Expr;
		}
		Expr()
			:ExprBase("")
		{
		}
	};

	template<> class Expr<Glib::ustring> : public ExprBase
	{
		friend Expr<Glib::ustring> substr(const Expr<Glib::ustring>& expr, long pos, long len);
		friend Expr<Glib::ustring> group_concat(const Expr<Glib::ustring>& expr);
	public:
		Expr(const Field<Glib::ustring>& field)
			:ExprBase(field.GetFieldName())
		{
		}

/*		Expr(const Expr<Glib::ustring>& expr)
			:ExprBase(expr.m_Expr)
		{
		}*/

		Expr(const Glib::ustring& value)
			:ExprBase(Field<Glib::ustring>::ToString(value))
		{
		}

		Expr(const char* value)
			:ExprBase(Field<Glib::ustring>::ToString(value))
		{
		}

		~Expr()
		{
		}

		Expr operator+(const Expr& expr) const
		{
			Expr res(*this);
			res.m_Expr += " || " + expr.m_Expr;
			return res;
		}
	protected:
		Glib::ustring& GetExpr()
		{
			return this->m_Expr;
		}
		Expr()
			:ExprBase("")
		{
		}
	};

	Expr<Glib::ustring> inline substr(const Expr<Glib::ustring>& expr, long pos, long len)
	{
		Expr<Glib::ustring> res;
		res.GetExpr() = "substr(" + expr.GetQuery() + "," + Field<long int>::ToString(pos) + "," + Field<long int>::ToString(len) + ")";
		return res;
	}

	Expr<Glib::ustring> inline group_concat(const Expr<Glib::ustring>& expr)
	{
		Expr<Glib::ustring> res;
		res.GetExpr() = "group_concat(" + expr.GetQuery() + ")";
		return res;
	}
}

#endif

