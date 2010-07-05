#ifndef _ORM_TABLE_H_
#define _ORM_TABLE_H_

#include <gtkmm/liststore.h>
#include "scheme.h"

namespace ORM
{
	class Table : public Gtk::ListStore
	{
	public:
		static Glib::RefPtr<Table> create(const Scheme& columns)
		{
			return Glib::RefPtr<Table>(new Table(columns));
		}

		const FieldBase& GetField(size_t i) const
		{
			return m_Scheme.GetField(i);
		}
	private:
		Table(const Scheme& columns)
			:Gtk::ListStore(columns),
			m_Scheme(columns)
		{
		}
		~Table()
		{
		}

		const Scheme& m_Scheme;
	};
}

#endif

