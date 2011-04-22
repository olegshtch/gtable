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
		InsertBase(Queryable &db, const Table& table, const FieldBase& f1)
			:QueryBase(db, "INSERT INTO " + table.GetTableName() + " DEFAULT VALUES"),
			m_Table(table)
		{
			m_Fields.push_back(&f1);
		}
		InsertBase(Queryable &db, const Table& table, const FieldBase& f1, const FieldBase& f2)
			:QueryBase(db, "INSERT INTO " + table.GetTableName() + " DEFAULT VALUES"),
			m_Table(table)
		{
			m_Fields.push_back(&f1);
			m_Fields.push_back(&f2);
		}

		InsertBase(Queryable &db, const Table& table, const FieldBase& f1, const FieldBase& f2, const FieldBase& f3)
			:QueryBase(db, "INSERT INTO " + table.GetTableName() + " DEFAULT VALUES"),
			m_Table(table)
		{
			m_Fields.push_back(&f1);
			m_Fields.push_back(&f2);
			m_Fields.push_back(&f3);
		}

		InsertBase(Queryable &db, const Table& table)
			:QueryBase(db, "INSERT INTO " + table.GetTableName() + " DEFAULT VALUES"),
			m_Table(table)
		{
			for(size_t i = 1; i < table.size(); ++ i)
			{
				m_Fields.push_back(&table.GetField(i));
			}
		}

		~InsertBase()
		{
		}

		void Values(const Gtk::TreeIter& row);
	private:
		const Table& m_Table;
		std::vector<const FieldBase*> m_Fields;
	};
}

#endif

