#ifndef _ORM_SELECT_H_
#define _ORM_SELECT_H_

#include "data.h"
#include "table.h"

namespace ORM
{
	class Connection;

	class SelectBase 
	{
	public:
		SelectBase(Connection &db, Glib::RefPtr<Data>& data, const Glib::ustring& fields)
			:m_Data(data),
			m_DB(db),
			m_Query(fields)
		{
		}
		~SelectBase();

		SelectBase& From(const Table& table1)
		{
			m_Query += " FROM " + table1.GetTableName();
			return *this;
		}
	private:
		Glib::RefPtr<Data>& m_Data;
		Connection &m_DB;
		Glib::ustring m_Query;

		SelectBase(const SelectBase&); // don't use
		SelectBase& operator=(const SelectBase&); // don't use
	};
}

#endif

