#ifndef _ORM_PRIMARY_KEY_
#define _ORM_PRIMARY_KEY_

#include "field.h"

namespace ORM
{
	class PrimaryKey
	{
	public:
		PrimaryKey(long value = -1)
			:m_Value(value)
		{
		}
		~PrimaryKey()
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

	template<> class Field<PrimaryKey> : public Gtk::TreeModelColumn<long>, public FieldBase
	{
	public:
		Field(const Table& tbl);
		~Field()
		{
		}
		void SetStrValue(Gtk::TreeIter &it, const Glib::ustring& str) const
		{
			long value;
			std::stringstream stream;
			stream << str;
			stream >> value;
			it->set_value(*this, value);
		}
		Glib::ustring GetStrValue(const Gtk::TreeIter &it) const
		{
			return Glib::ustring::format(it->get_value(*this));
		}
		Glib::ustring GetDefinition() const
		{
			return m_FieldName + " INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL"; 
		}
	};
}

#endif

