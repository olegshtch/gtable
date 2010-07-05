#ifndef _ORM_FIELD_H_
#define _ORM_FIELD_H_

#include <gtkmm/treemodelcolumn.h>
#include <sstream>

namespace ORM
{
	class FieldBase
	{
	public:
		FieldBase()
		{
		}
		~FieldBase()
		{
		}
		virtual void SetStrValue(Gtk::TreeIter &it, const Glib::ustring& str) const = 0;
		virtual Glib::ustring GetStrValue(Gtk::TreeIter &it) const = 0;
	private:
	};

	template<class T> class Field : public FieldBase
	{
	public:
		Field()
		{
		}
		~Field()
		{
		}
		void SetStrValue(Gtk::TreeIter &it, const Glib::ustring& str) const
		{
			T value;
			std::stringstream stream;
			stream << str;
			stream >> value;
			it->set_value(m_Column, value);
		}
		Glib::ustring GetStrValue(Gtk::TreeIter &it) const
		{
			return Glib::ustring::format(it->get_value(m_Column));
		}
		operator const Gtk::TreeModelColumn<T>&() const
		{
			return m_Column;
		}
	private:
		Gtk::TreeModelColumn<T> m_Column;
	};
#if 0
	class Field<long> : public FieldBase
	{
	public:
		Field(Gtk::TreeModelColumn<long> &column)
			:m_Column(column)
		{
		}
		~Field()
		{
		}
		void SetStrValue(Gtk::TreeIter &it, const Glib::ustring& str)
		{
			it->set_value(m_Column, atol(str));
		}
		Glib::ustring GetStrValue(Gtk::TreeIter &it)
		{
			return 
		}
	private:
	};
#endif
}

#endif

