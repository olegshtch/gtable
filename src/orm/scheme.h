#ifndef _ORM_SCHEME_H_
#define _ORM_SCHEME_H_

#include "field.h"

namespace ORM
{
	class Scheme : public Gtk::TreeModelColumnRecord
	{
	public:
		Scheme()
		{
		}
		~Scheme()
		{
		}
		const FieldBase& GetField(size_t i) const
		{
			return *(m_Fields[i]);
		}

		template <class T> void add(Field<T>& field)
		{
			Gtk::TreeModelColumnRecord::add(field);
			m_Fields.push_back(&field);
		}
	private:
		std::vector<const FieldBase*> m_Fields;
	};
};

#endif

