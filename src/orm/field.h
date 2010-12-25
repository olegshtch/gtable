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
		virtual ~FieldBase()
		{
		}
		virtual void SetStrValue(Gtk::TreeIter &it, const Glib::ustring& str) const = 0;
		virtual Glib::ustring GetStrValue(Gtk::TreeIter &it) const = 0;
	private:
	};

	template<class T> class Field : public Gtk::TreeModelColumn<T>, public FieldBase
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
			it->set_value(*this, value);
		}
		Glib::ustring GetStrValue(Gtk::TreeIter &it) const
		{
			return Glib::ustring::format(it->get_value(*this));
		}
#if 0
		operator const Gtk::TreeModelColumn<T>&() const
		{
			return m_Column;
		}
		const Gtk::TreeModelColumn<T>& GetColumn() const
		{
			return m_Column;
		}
		operator Gtk::TreeModelColumnBase&()
		{
			return m_Column;
		}

		Gtk::TreeModelColumn<T>& GetColumn()
		{
			return m_Column;
		}
#endif
	private:
//		Gtk::TreeModelColumn<T> m_Column;
	};
#if 1
	template<> class Field<Glib::ustring> : public Gtk::TreeModelColumn<Glib::ustring>, public FieldBase
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
			it->set_value(*this, str);
		}
		Glib::ustring GetStrValue(Gtk::TreeIter &it) const
		{
			return it->get_value(*this);
		}
	private:
	};
#endif
}

#endif

