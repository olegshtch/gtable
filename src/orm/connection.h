#ifndef _ORM_CONNECTION_H_
#define _ORM_CONNECTION_H_

#include <sqlite3.h>
#include <memory>
#include "data.h"
#include "table.h"
#include "select.h"

namespace ORM
{
	const bool IF_NOT_EXIST = false;
	const bool IF_EXIST = true;

	class Connection
	{
	public:
		Connection(const Glib::ustring &file, bool create_new);
		~Connection();

		void CreateTable(const Table &table, bool is_exist = IF_EXIST)
		{
			if(is_exist)
			{
				SQLExec0("DROP TABLE " + table.GetTableName());
			}
			SQLExec0(table.GetSqlCreateString());
		}

		// Select family
		std::auto_ptr<SelectBase> Select(Glib::RefPtr<Data> &data)
		{
			return std::auto_ptr<SelectBase>(new SelectBase(*this, data, "*"));
		}
		
		std::auto_ptr<SelectBase> Select(Glib::RefPtr<Data> &data, const FieldBase& f1)
		{
			return std::auto_ptr<SelectBase>(new SelectBase(*this, data, f1.GetFieldName()));
		}

		std::auto_ptr<SelectBase> Select(Glib::RefPtr<Data> &data, const FieldBase& f1, const FieldBase& f2)
		{
			return std::auto_ptr<SelectBase>(new SelectBase(*this, data, f1.GetFieldName() + "," + f2.GetFieldName()));
		}
	
		void SQLExec0(const Glib::ustring &sql);
		void SQLExec(const Glib::ustring &sql, Glib::RefPtr<Data>& data);
		void SQLExecOwn(const Glib::ustring &sql, int (*callback)(void*,int,char**,char**), void* data);

	private:
		static int CallBack0(void *self_ptr, int argc, char **argv, char **col_name);
		static int CallBack(void *self_ptr, int argc, char **argv, char **col_name);
		sqlite3 *m_SQLite;
	};
}

#endif

