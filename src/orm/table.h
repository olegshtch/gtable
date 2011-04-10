#ifndef _ORM_TABLE_H_
#define _ORM_TABLE_H_

#include <set>
#include "scheme.h"
#include "primary_key.h"

namespace ORM
{
	class Connection;

	class Table : public Scheme
	{
	public:
		Table(const Glib::ustring& table_name)
			:m_TableName(table_name),
			fId(*this)
		{
			if(! s_Tables)
			{
				s_Tables = new std::set<const Table*>();
			}
			s_Tables->insert(this);
			add(fId);
		}

		~Table()
		{
			s_Tables->erase(this);
			if(s_Tables->empty())
			{
				delete s_Tables;
				s_Tables = NULL;
			}
		}

		Glib::ustring GetSqlCreateString() const;
		const Glib::ustring& GetTableName() const
		{
			return m_TableName;
		}

		static void InitTables(ORM::Connection &db);
	private:
		Glib::ustring m_TableName;
		static std::set<const Table*> *s_Tables;
	public:
		Field<PrimaryKey> fId;
	};
}

#endif

