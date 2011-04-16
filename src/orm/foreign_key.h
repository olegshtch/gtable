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

	template<> class Field<ForeignKey> : public Field<PrimaryKey>
	{
	public:
		Field(const Table& tbl);
		~Field()
		{
		}
		Glib::ustring GetDefinition() const
		{
			return m_FieldName + " REFERENCES " + m_ForeignTable + " (" + m_FieldName + ") ON DELETE CASCADE ON UPDATE CASCADE NOT NULL";
		}
		static Glib::ustring ToString(const ForeignKey& value)
		{
			return Field<PrimaryKey>::ToString(static_cast<long>(value));
		}
	private:
		const Glib::ustring& m_ForeignTable;
	};
}

#endif

