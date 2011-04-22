#ifndef _ORM_CONNECTION_H_
#define _ORM_CONNECTION_H_

#include <sqlite3.h>
#include "queryable.h"

namespace ORM
{
	class Connection : public Queryable
	{
		friend class Query;
	public:
		Connection(const Glib::ustring &file, bool create_new);
		~Connection();
		void MoveTo(const Glib::ustring& dbname);

	private:
		void SQLExec0(const Glib::ustring &sql);
		void SQLExec(const Glib::ustring &sql, Glib::RefPtr<Data>& data);
		void SQLExecOwn(const Glib::ustring &sql, int (*callback)(void*,int,char**,char**), void* data);

		static int CallBack0(void *self_ptr, int argc, char **argv, char **col_name);
		static int CallBack(void *self_ptr, int argc, char **argv, char **col_name);
		static void Trace(void *, const char *sql);
		sqlite3 *m_SQLite;
	};
}

#endif

