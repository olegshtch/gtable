#ifndef _ORM_QUERYABLE_H_
#define _ORM_QUERYABLE_H_

#include <memory>
#include "data.h"
#include "table.h"
#include "select.h"
#include "insert.h"
#include "update.h"
#include "delete.h"
#include "expr.h"

namespace ORM
{
	const bool IF_NOT_EXIST = false;
	const bool IF_EXIST = true;

	class Queryable
	{
	public:
		Queryable()
		{
		}
		virtual ~Queryable()
		{
		}

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
		
		std::auto_ptr<SelectBase> Select(Glib::RefPtr<Data> &data, const ExprBase& f1)
		{
			return std::auto_ptr<SelectBase>(new SelectBase(*this, data, f1.GetQuery()));
		}

		std::auto_ptr<SelectBase> Select(Glib::RefPtr<Data> &data, const ExprBase& f1, const ExprBase& f2)
		{
			return std::auto_ptr<SelectBase>(new SelectBase(*this, data, f1.GetQuery() + "," + f2.GetQuery()));
		}

		std::auto_ptr<SelectBase> Select(Glib::RefPtr<Data> &data, const ExprBase& f1, const ExprBase& f2, const ExprBase& f3)
		{
			return std::auto_ptr<SelectBase>(new SelectBase(*this, data, f1.GetQuery() + "," + f2.GetQuery() + "," + f3.GetQuery()));
		}
		std::auto_ptr<SelectBase> Select(Glib::RefPtr<Data> &data, const ExprBase& f1, const ExprBase& f2, const ExprBase& f3, const ExprBase& f4)
		{
			return std::auto_ptr<SelectBase>(new SelectBase(*this, data, f1.GetQuery() + "," + f2.GetQuery() + "," + f3.GetQuery() + "," + f4.GetQuery()));
		}
		std::auto_ptr<SelectBase> Select(Glib::RefPtr<Data> &data, const ExprBase& f1, const ExprBase& f2, const ExprBase& f3, const ExprBase& f4, const ExprBase& f5)
		{
			return std::auto_ptr<SelectBase>(new SelectBase(*this, data, f1.GetQuery() + "," + f2.GetQuery() + "," + f3.GetQuery() + "," + f4.GetQuery() + "," + f5.GetQuery()));
		}
		std::auto_ptr<SelectBase> Select(Glib::RefPtr<Data> &data, const ExprBase& f1, const ExprBase& f2, const ExprBase& f3, const ExprBase& f4, const ExprBase& f5, const ExprBase& f6)
		{
			return std::auto_ptr<SelectBase>(new SelectBase(*this, data, f1.GetQuery() + "," + f2.GetQuery() + "," + f3.GetQuery() + "," + f4.GetQuery() + "," + f5.GetQuery() + "," + f6.GetQuery()));
		}

		// Select DISTINCT family
		std::auto_ptr<SelectBase> SelectDistinct(Glib::RefPtr<Data> &data)
		{
			return std::auto_ptr<SelectBase>(new SelectBase(*this, data, "*", true));
		}
		
		std::auto_ptr<SelectBase> SelectDistinct(Glib::RefPtr<Data> &data, const ExprBase& f1)
		{
			return std::auto_ptr<SelectBase>(new SelectBase(*this, data, f1.GetQuery(), true));
		}

		std::auto_ptr<SelectBase> SelectDistinct(Glib::RefPtr<Data> &data, const ExprBase& f1, const ExprBase& f2)
		{
			return std::auto_ptr<SelectBase>(new SelectBase(*this, data, f1.GetQuery() + "," + f2.GetQuery(), true));
		}

		std::auto_ptr<SelectBase> SelectDistinct(Glib::RefPtr<Data> &data, const ExprBase& f1, const ExprBase& f2, const ExprBase& f3)
		{
			return std::auto_ptr<SelectBase>(new SelectBase(*this, data, f1.GetQuery() + "," + f2.GetQuery() + "," + f3.GetQuery(), true));
		}
		std::auto_ptr<SelectBase> SelectDistinct(Glib::RefPtr<Data> &data, const ExprBase& f1, const ExprBase& f2, const ExprBase& f3, const ExprBase& f4)
		{
			return std::auto_ptr<SelectBase>(new SelectBase(*this, data, f1.GetQuery() + "," + f2.GetQuery() + "," + f3.GetQuery() + "," + f4.GetQuery(), true));
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
		std::auto_ptr<InsertBase> InsertInto(const Table& table, const FieldBase& f1, const FieldBase& f2, const FieldBase& f3)
		{
			return std::auto_ptr<InsertBase>(new InsertBase(*this, table, f1, f2, f3));
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

		virtual void SQLExec0(const Glib::ustring &sql) = 0;
		virtual void SQLExec(const Glib::ustring &sql, Glib::RefPtr<Data>& data) = 0;
	};
}

#endif

