#ifndef _ORM_TABLE_H_
#define _ORM_TABLE_H_

#include <set>
#include "scheme.h"
#include "primary_key.h"

namespace ORM
{
	class Table : public Scheme
	{
	public:
		typedef std::set<const Table*>::const_iterator const_iterator;

		Table(const Glib::ustring& table_name)
			:m_TableName(table_name),
			fId(*this)
		{
			s_Tables.insert(this);
			add(fId);
		}

		~Table()
		{
			s_Tables.erase(this);
		}

		Glib::ustring GetSqlCreateString() const;
		const Glib::ustring& GetTableName() const
		{
			return m_TableName;
		}

		static std::set<const Table*>::const_iterator begin()
		{
			return s_Tables.begin();
		}

		static std::set<const Table*>::const_iterator end()
		{
			return s_Tables.end();
		}
	private:
		Glib::ustring m_TableName;
		static std::set<const Table*> s_Tables;
	public:
		Field<PrimaryKey> fId;
	};
}

#endif

