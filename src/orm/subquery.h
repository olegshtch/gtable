#ifndef _ORM_SUBQUERY_H_
#define _ORM_SUBQUERY_H_

#include "queryable.h"

namespace ORM
{
	class Subquery : public Queryable
	{
	public:
		Subquery()
		{
		}
		~Subquery()
		{
		}
		Glib::ustring GetQuery() const
		{
			return m_Query;
		}
		void SQLExec0(const Glib::ustring &sql)
		{
			m_Query = sql;
		}
		void SQLExec(const Glib::ustring &sql, Glib::RefPtr<Data>&)
		{
			m_Query = sql;
		}
	private:
		Glib::ustring m_Query;
	};
}

#endif

