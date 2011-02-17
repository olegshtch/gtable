#ifndef _ORM_TABLE_H_
#define _ORM_TABLE_H_

#include <gtkmm/liststore.h>
#include "scheme.h"

namespace ORM
{
	class Data : public Gtk::ListStore
	{
	public:
		static Glib::RefPtr<Data> create(const Scheme& columns)
		{
			return Glib::RefPtr<Data>(new Data(columns));
		}

		const FieldBase& GetField(size_t i) const
		{
			return m_Scheme.GetField(i);
		}
	private:
		Data(const Scheme& columns)
			:Gtk::ListStore(columns),
			m_Scheme(columns)
		{
		}
		~Data()
		{
		}

		const Scheme& m_Scheme;
	};
}

#endif

