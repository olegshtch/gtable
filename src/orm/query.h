#ifndef _ORM_QUERY_H_
#define _ORM_QUERY_H_

#include <glibmm/ustring.h>
#include "data.h"

namespace ORM
{
	class Queryable;

	class QueryBase
	{
	public:
		QueryBase(Queryable& db, const Glib::ustring& str = Glib::ustring(), Glib::RefPtr<Data> data = Glib::RefPtr<Data>())
			:m_Query(str), m_DB(db), m_Data(data)
		{
		}
		~QueryBase();
	protected:
		Glib::ustring m_Query;
	private:
		Queryable &m_DB;
		Glib::RefPtr<Data> m_Data;

		QueryBase(const QueryBase&); // don't use
		QueryBase& operator=(const QueryBase&); // don't use
	};
}

#endif

