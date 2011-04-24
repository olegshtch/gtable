#ifndef _ORM_SELECT_H_
#define _ORM_SELECT_H_

#include "query.h"
#include "table.h"
#include "where.h"

namespace ORM
{
	class SelectBase : public QueryBase
	{
	public:
		SelectBase(Queryable &db, Glib::RefPtr<Data>& data, const Glib::ustring& fields)
			:QueryBase(db, "SELECT ", data),
			m_Fields(fields)
		{
		}
		~SelectBase()
		{
		}

		SelectBase* From(const Table& table1)
		{
			m_Query += m_Fields + " FROM " + table1.GetTableName();
			return this;
		}
		SelectBase* From(const Table& table1, const Table& table2)
		{
			m_Query += m_Fields + " FROM " + table1.GetTableName() + "," + table2.GetTableName();
			return this;
		}
		SelectBase* From(const Table& table1, const Table& table2, const Table& table3)
		{
			m_Query += m_Fields + " FROM " + table1.GetTableName() + "," + table2.GetTableName() + "," + table3.GetTableName();
			return this;
		}
		SelectBase* From(const Table& table1, const Table& table2, const Table& table3, const Table& table4)
		{
			m_Query += m_Fields + " FROM " + table1.GetTableName() + "," + table2.GetTableName() + "," + table3.GetTableName() + "," + table4.GetTableName();
			return this;
		}

		void Where(const WhereBase& where)
		{
			m_Query += " WHERE " + where.GetQuery();
		}
	private:
		Glib::ustring m_Fields;
	};
}

#endif

