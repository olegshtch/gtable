#ifndef _ORM_FOREIGN_KEY_H_
#define _ORM_FOREIGN_KEY_H_

#include "primary_key.h"

namespace ORM
{
	class ForeignKey
	{
	public:
		ForeignKey(long value = -1)
			:m_Value(value)
		{
		}
		~ForeignKey()
		{
		}
		operator long() const
		{
			return m_Value;
		}
	private:
		long m_Value;
	};

	class Table;

	template<> class Field<ForeignKey> : public Field<long int>
	{
	public:
		Field(const Table& tbl, bool not_null = true);
		Field(const Glib::ustring& field_name, const Table& tbl, bool not_null = true);
		~Field()
		{
		}
		Glib::ustring GetDefinition() const
		{
			return m_FieldName + " REFERENCES " + m_ForeignTable + " (id_" + m_ForeignTable + ") ON DELETE CASCADE ON UPDATE CASCADE " + (m_NotNull ? "NOT NULL" : "");
		}
		static Glib::ustring ToString(const ForeignKey& value)
		{
			return Field<PrimaryKey>::ToString(static_cast<long>(value));
		}
		virtual void SetTableName(const Glib::ustring& table_name)
		{
			m_TableName = table_name;
		}
	private:
		const Glib::ustring& m_ForeignTable;
		bool m_NotNull;
	};
}

#endif

