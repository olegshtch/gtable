#ifndef _ORM_WHERE_H_
#define _ORM_WHERE_H_

#include <glibmm/ustring.h>

namespace ORM
{
	class WhereBase
	{
	public:
		WhereBase(const Glib::ustring& str = Glib::ustring())
			:m_Query(str)
		{
		}
		~WhereBase()
		{
		}

		const Glib::ustring& GetQuery() const
		{
			return m_Query;
		}
	protected:
		Glib::ustring m_Query;
	};

	class Eq : public WhereBase
	{
	public:
		Eq(const FieldBase& field, const Gtk::TreeIter& row)
			:WhereBase(field.GetFieldName() + "=" + field.GetStrValue(row))
		{
		}
		~Eq()
		{
		}
	};
}

#endif 

