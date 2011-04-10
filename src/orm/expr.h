#ifndef _ORM_EXPR_H_
#define _ORM_EXPR_H_

#include "field.h"

namespace ORM
{
	template<class T> class Expr
	{
		friend class Field<T>;
	public:
		Expr(const Field<T>& field)
			:m_Expr(field.GetFieldName())
		{
		}

		Expr(const Expr<T>& expr)
			:m_Expr(expr.m_Expr)
		{
		}

		Expr(const T& value)
			:m_Expr(Field<T>::ToString(value))
		{
		}

		~Expr()
		{
		}

		operator Field<T>() const
		{
			return Field<T>(m_Expr);
		}
	private:
		Glib::ustring m_Expr;
	};

	template<> class Expr<Glib::ustring>
	{
		friend class Field<Glib::ustring>;
	public:
		Expr(const Field<Glib::ustring>& field)
			:m_Expr(field.GetFieldName())
		{
		}

		Expr(const Expr<Glib::ustring>& expr)
			:m_Expr(expr.m_Expr)
		{
		}

		Expr(const Glib::ustring& value)
			:m_Expr(Field<Glib::ustring>::ToString(value))
		{
		}

		Expr(const char* value)
			:m_Expr(Field<Glib::ustring>::ToString(value))
		{
		}

		~Expr()
		{
		}

		Expr operator+(const Expr& expr) const
		{
			Expr res(*this);
			res.m_Expr += " + " + expr.m_Expr;
			return res;
		}

		operator Field<Glib::ustring>() const
		{
			return Field<Glib::ustring>(m_Expr);
		}
	private:
		Glib::ustring m_Expr;
	};
}

#endif

