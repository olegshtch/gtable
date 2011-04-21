#ifndef _ORM_CONNECTION_H_
#define _ORM_CONNECTION_H_

#include <sqlite3.h>
#include <memory>
#include "data.h"
#include "table.h"
#include "select.h"
#include "insert.h"
#include "update.h"
#include "delete.h"

namespace ORM
{
	const bool IF_NOT_EXIST = false;
	const bool IF_EXIST = true;

	class Connection
	{
	public:
		Connection(const Glib::ustring &file, bool create_new);
		~Connection();
		void MoveTo(const Glib::ustring& dbname);

		void CreateTable(const Table &table, bool is_exist = IF_EXIST)
		{
			if(is_exist)
			{
				SQLExec0("DROP TABLE IF EXISTS " + table.GetTableName());
			}
			SQLExec0(table.GetSqlCreateString());
		}

		void CreateTrigger(const Glib::ustring& name, const Glib::ustring& body)
		{
			SQLExec0("CREATE TRIGGER " + name + " " + body);
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

		// Insert family
		std::auto_ptr<InsertBase> InsertInto(const Table& table)
		{
			return std::auto_ptr<InsertBase>(new InsertBase(*this, table));
		}
		std::auto_ptr<InsertBase> InsertInto(const Table& table, const FieldBase& f1)
		{
			return std::auto_ptr<InsertBase>(new InsertBase(*this, table, f1));
		}
		std::auto_ptr<InsertBase> InsertInto(const Table& table, const FieldBase& f1, const FieldBase& f2)
		{
			return std::auto_ptr<InsertBase>(new InsertBase(*this, table, f1, f2));
		}

		// Update family
		std::auto_ptr<UpdateBase> Update(const Table& table)
		{
			return std::auto_ptr<UpdateBase>(new UpdateBase(*this, table));
		}

		// Delete family
		std::auto_ptr<DeleteBase> DeleteFrom(const Table& table)
		{
			return std::auto_ptr<DeleteBase>(new DeleteBase(*this, table));
		}
	
		void SQLExec0(const Glib::ustring &sql);
		void SQLExec(const Glib::ustring &sql, Glib::RefPtr<Data>& data);
		void SQLExecOwn(const Glib::ustring &sql, int (*callback)(void*,int,char**,char**), void* data);

	private:
		static int CallBack0(void *self_ptr, int argc, char **argv, char **col_name);
		static int CallBack(void *self_ptr, int argc, char **argv, char **col_name);
		static void Trace(void *, const char *sql);
		sqlite3 *m_SQLite;
	};
}

#endif

