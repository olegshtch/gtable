#ifndef _ORM_FIELD_H_
#define _ORM_FIELD_H_

#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treeiter.h>
#include <glibmm/ustring.h>
#include <sstream>

namespace ORM
{
	class FieldBase
	{
	public:
		FieldBase(const Glib::ustring& name)
			:m_FieldName(name)
		{
		}
		virtual ~FieldBase()
		{
		}
		virtual void SetStrValue(Gtk::TreeIter &it, const Glib::ustring& str) const = 0;
		virtual Glib::ustring GetStrValue(const Gtk::TreeIter &it) const = 0;
		virtual Glib::ustring GetDefinition() const = 0;
		const Glib::ustring& GetFieldName() const
		{
			return m_FieldName;
		}
	protected:
		Glib::ustring m_FieldName;
	};

	template<class T> class Field : public Gtk::TreeModelColumn<T>, public FieldBase
	{
	public:
		Field(const Glib::ustring& name)
			:FieldBase(name)
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
		Glib::ustring GetStrValue(const Gtk::TreeIter &it) const
		{
			return Glib::ustring::format(it->get_value(*this));
		}
	};

	template<> class Field<Glib::ustring> : public Gtk::TreeModelColumn<Glib::ustring>, public FieldBase
	{
	public:
		Field(const Glib::ustring& name)
			:FieldBase(name)
		{
		}
		~Field()
		{
		}
		void SetStrValue(Gtk::TreeIter &it, const Glib::ustring& str) const
		{
			it->set_value(*this, str);
		}
		Glib::ustring GetStrValue(const Gtk::TreeIter &it) const
		{
			Glib::ustring res = "\"";
			res += Glib::strescape(it->get_value(*this));
			res += "\"";
			return res;
		}
		Glib::ustring GetDefinition() const
		{
			return m_FieldName + " TEXT NOT NULL"; 
		}
	};

	template<> class Field<long int> : public Gtk::TreeModelColumn<long int>, public FieldBase
	{
	public:
		Field(const Glib::ustring& name)
			:FieldBase(name)
		{
		}
		~Field()
		{
		}
		void SetStrValue(Gtk::TreeIter &it, const Glib::ustring& str) const
		{
			long int value;
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
			return m_FieldName + " INT NOT NULL"; 
		}
	};

	template<> class Field<bool> : public Gtk::TreeModelColumn<bool>, public FieldBase
	{
	public:
		Field(const Glib::ustring& name)
			:FieldBase(name)
		{
		}
		~Field()
		{
		}
		void SetStrValue(Gtk::TreeIter &it, const Glib::ustring& str) const
		{
			int value;
			std::stringstream stream;
			stream << str;
			stream >> value;
			it->set_value(*this, (bool)value);
		}
		Glib::ustring GetStrValue(const Gtk::TreeIter &it) const
		{
			return it->get_value(*this) ? "1" : "0";
		}
		Glib::ustring GetDefinition() const
		{
			return m_FieldName + " TINYINT(1) NOT NULL"; 
		}
	};

}

#endif

