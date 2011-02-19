#ifndef _ORM_TABLE_H_
#define _ORM_TABLE_H_

#include "scheme.h"
#include "primary_key.h"

namespace ORM
{
	class Table : public Scheme
	{
	public:
		Table(const Glib::ustring& table_name)
			:m_TableName(table_name),
			fId(*this)
		{
			add(fId);
		}

		~Table()
		{
		}

		Glib::ustring GetSqlCreateString() const;
		const Glib::ustring& GetTableName() const
		{
			return m_TableName;
		}
	private:
		Glib::ustring m_TableName;

	public:
		Field<PrimaryKey> fId;
	};
}

#endif

