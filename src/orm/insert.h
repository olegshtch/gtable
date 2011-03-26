#ifndef _ORM_INSERT_H_
#define _ORM_INSERT_H_

#include <glibmm/ustring.h>
#include "query.h"
#include "table.h"

namespace ORM
{
	class Connection;

	class InsertBase : public QueryBase
	{
	public:
		InsertBase(Connection &db, const Table& table)
			:QueryBase(db, "INSERT INTO " + table.GetTableName()),
			m_Table(table)
		{
		}
		~InsertBase()
		{
		}

		void Values(const Gtk::TreeIter& row);
	private:
		const Table& m_Table;
	};
}

#endif

