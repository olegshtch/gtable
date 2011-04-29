#ifndef _ORM_WHERE_H_
#define _ORM_WHERE_H_

#include <glibmm/ustring.h>
#include "expr.h"

namespace ORM
{
	class WhereBase
	{
	public:
		WhereBase(const Glib::ustring& str = Glib::ustring())
			:m_Query(str)
		{
		}
		~WhereBase()
		{
		}

		const Glib::ustring& GetQuery() const
		{
			return m_Query;
		}

		WhereBase operator &&(const WhereBase& op2)
		{
			return WhereBase(this->m_Query + " AND " + op2.m_Query);
		}
		WhereBase operator ||(const WhereBase& op2)
		{
			return WhereBase("(" + this->m_Query + " AND " + op2.m_Query + ")");
		}
	protected:
		Glib::ustring m_Query;
	};

	class Eq : public WhereBase
	{
	public:
		Eq(const FieldBase& field, const Gtk::TreeIter& row)
			:WhereBase(field.GetFieldName() + "=" + field.GetStrValue(row))
		{
		}
		Eq(const FieldBase& field, const FieldBase& field2)
			:WhereBase(field.GetFieldName() + "=" + field2.GetFieldName())
		{
		}
		template<class T> Eq(const Field<T>& field, const T& value)
			:WhereBase(field.GetFieldName() + "=" + Field<T>::ToString(value))
		{
		}
		~Eq()
		{
		}
	};

	class NonEq : public WhereBase
	{
		NonEq(const FieldBase& field, const Gtk::TreeIter& row)
			:WhereBase(field.GetFieldName() + "=" + field.GetStrValue(row))
		{
		}
		NonEq(const FieldBase& field, const FieldBase& field2)
			:WhereBase(field.GetFieldName() + "=" + field2.GetFieldName())
		{
		}
		template<class T> NonEq(const Field<T>& field, const T& value)
			:WhereBase(field.GetFieldName() + "=" + Field<T>::ToString(value))
		{
		}
		~NonEq()
		{
		}
	};

	class Greater : public WhereBase
	{
	public:
		template<class T> Greater(const Field<T>& field, const T& value)
			:WhereBase(field.GetFieldName() + ">" + Field<T>::ToString(value))
		{
		}
	};

	class Subquery;

	class NotIn : public WhereBase
	{
	public:
		NotIn(const ExprBase& expr, const Subquery& subquery);
	};
}

#endif 

