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

	template <class T1, class T2 = void, class T3 = void> class Tuple : public Scheme
	{
	public:
		Tuple()
		{
			add<T1>(f1);
			add<T2>(f2);
			add<T3>(f3);
		}
		~Tuple()
		{
		}
		Field<T1> f1;
		Field<T2> f2;
		Field<T3> f3;
	};
	
	template <class T1, class T2> class Tuple<T1, T2, void> : public Scheme
	{
	public:
		Tuple()
		{
			add<T1>(f1);
			add<T2>(f2);
		}
		~Tuple()
		{
		}
		Field<T1> f1;
		Field<T2> f2;
	};

	template <class T1> class Tuple<T1, void, void> : public Scheme
	{
	public:
		Tuple()
		{
			add<T1>(f1);
		}
		~Tuple()
		{
		}
		Field<T1> f1;
	};
}

#endif

