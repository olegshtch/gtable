#ifndef _ORM_TABLE_H_
#define _ORM_TABLE_H_

#include <set>
#include "scheme.h"
#include "primary_key.h"

namespace ORM
{
	class Connection;

	class Table : public Scheme
	{
	public:
		Table(const Glib::ustring& table_name)
			:m_TableName(table_name),
			fId()
		{
			if(! s_Tables)
			{
				s_Tables = new std::set<const Table*>();
			}
			s_Tables->insert(this);
			add(fId);
		}

		~Table()
		{
			s_Tables->erase(this);
			if(s_Tables->empty())
			{
				delete s_Tables;
				s_Tables = NULL;
			}
		}

		Glib::ustring GetSqlCreateString() const;
		const Glib::ustring& GetTableName() const
		{
			return m_TableName;
		}

		template <class T> void add(Field<T>& field)
		{
			Scheme::add(field);
			field.SetTableName(m_TableName);
		}

		void Unique(const FieldBase& field1)
		{
			m_Constraints.push_back("UNIQUE (" + field1.GetSmallFieldName() + ")");
		}
		void Unique(const FieldBase& field1, const FieldBase& field2)
		{
			m_Constraints.push_back("UNIQUE (" + field1.GetSmallFieldName() + "," + field2.GetSmallFieldName() + ")");
		}
		void Unique(const FieldBase& field1, const FieldBase& field2, const FieldBase& field3)
		{
			m_Constraints.push_back("UNIQUE (" + field1.GetSmallFieldName() + "," + field2.GetSmallFieldName() + "," + field3.GetSmallFieldName() + ")");
		}
		void Unique(const FieldBase& field1, const FieldBase& field2, const FieldBase& field3, const FieldBase& field4)
		{
			m_Constraints.push_back("UNIQUE (" + field1.GetSmallFieldName() + "," + field2.GetSmallFieldName() + "," + field3.GetSmallFieldName() + "," + field4.GetSmallFieldName() + ")");
		}
		void Unique(const FieldBase& field1, const FieldBase& field2, const FieldBase& field3, const FieldBase& field4, const FieldBase& field5)
		{
			m_Constraints.push_back("UNIQUE (" + field1.GetSmallFieldName() + "," + field2.GetSmallFieldName() + "," + field3.GetSmallFieldName() + "," + field4.GetSmallFieldName() + "," + field5.GetSmallFieldName() + ")");
		}

		static void InitTables(ORM::Connection &db);
	private:
		Glib::ustring m_TableName;
		std::vector<Glib::ustring> m_Constraints;
		static std::set<const Table*> *s_Tables;
	public:
		Field<PrimaryKey> fId;
	};
}

#endif

