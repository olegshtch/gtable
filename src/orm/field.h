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
		Glib::ustring GetFieldName() const
		{
			return m_TableName.empty() ? m_FieldName : (m_TableName + "." + m_FieldName);
		}
		Glib::ustring GetSmallFieldName() const
		{
			return m_FieldName;
		}
		virtual void SetTableName(const Glib::ustring& table_name)
		{
			m_TableName = table_name;
			m_FieldName = table_name + "_" + m_FieldName;
		}
	protected:
		Glib::ustring m_FieldName;
		Glib::ustring m_TableName;
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
		static Glib::ustring ToString(const T& value)
		{
			return Glib::ustring::format(value);
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
			res += it->get_value(*this);
			res += "\"";
			return res;
		}
		Glib::ustring GetDefinition() const
		{
			return m_FieldName + " TEXT NOT NULL"; 
		}
		static Glib::ustring ToString(const Glib::ustring& value)
		{
			Glib::ustring res = "\"";
			res += value;
			res += "\"";
			return res;
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
			std::stringstream stream;
			stream << it->get_value(*this);
			return stream.str();
			//return Glib::ustring::format(it->get_value(*this));
		}
		Glib::ustring GetDefinition() const
		{
			return m_FieldName + " INT NOT NULL"; 
		}
		static Glib::ustring ToString(long int value)
		{
			std::stringstream stream;
			stream << value;
			return stream.str();
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
			return m_FieldName + " TINYINT(1) NOT NULL DEFAULT 0"; 
		}
		static Glib::ustring ToString(bool value)
		{
			return value ? "1" : "0";
		}
	};

}

#endif

