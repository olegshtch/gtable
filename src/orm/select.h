#ifndef _ORM_SELECT_H_
#define _ORM_SELECT_H_

#include "query.h"
#include "table.h"

namespace ORM
{
	class Connection;

	class SelectBase : public QueryBase
	{
	public:
		SelectBase(Connection &db, Glib::RefPtr<Data>& data, const Glib::ustring& fields)
			:QueryBase(db, "SELECT ", data),
			m_Fields(fields)
		{
		}
		~SelectBase()
		{
		}

		SelectBase& From(const Table& table1)
		{
			m_Query += m_Fields + " FROM " + table1.GetTableName();
			return *this;
		}
	private:
		Glib::ustring m_Fields;
	};
}

#endif

