#include <glibmm/error.h>
#include <iostream>
#include "connection.h"
#include "../shared.h"

ORM::Connection::Connection(const Glib::ustring &file, bool create_new)
	:m_SQLite(0)
{
	int flags = SQLITE_OPEN_READWRITE;
	if(create_new)
	{
		flags |= SQLITE_OPEN_CREATE;
	}
	int sql_answer = sqlite3_open_v2(file.c_str(), &m_SQLite, flags, 0);
	if(sql_answer != SQLITE_OK)
	{
		sqlite3_close(m_SQLite);
		throw Glib::Error(1, 0, sqlite3_errmsg(m_SQLite));
	}

	//sqlite3_trace(m_SQLite, ORM::Connection::Trace, NULL);

	SQLExec0("PRAGMA foreign_key = ON");
	SQLExec0("PRAGMA recursive_triggers = ON");
}

ORM::Connection::~Connection()
{
	if(m_SQLite)
	{
		sqlite3_stmt *sql_stmt;
		do
		{
			sql_stmt = sqlite3_next_stmt(m_SQLite, 0);
			if(!sql_stmt)
			{
				break;
			}
			sqlite3_finalize(sql_stmt);
		}
		while(sql_stmt);
		sqlite3_close(m_SQLite);
	}
	std::clog << "SQLite finalized." << std::endl;
}

void ORM::Connection::MoveTo(const Glib::ustring& dbname)
{
	sqlite3 *new_handle;
	int sqlite_answer = sqlite3_open(dbname.c_str(), &new_handle);
	if(sqlite_answer != SQLITE_OK)
	{
		sqlite3_close(new_handle);
		throw Glib::Error(1, 0, sqlite3_errmsg(new_handle));
	}
	sqlite3_backup *backup_handle = sqlite3_backup_init(new_handle, "main", m_SQLite, "main");
	if(! backup_handle)
	{
		sqlite3_close(new_handle);
		throw Glib::Error(1, 0, sqlite3_errmsg(new_handle));
	}
	sqlite3_backup_step(backup_handle, -1);
	sqlite3_backup_finish(backup_handle);
	sqlite_answer = sqlite3_errcode(new_handle);
	if(sqlite_answer != SQLITE_OK)
	{
		sqlite3_close(new_handle);
		throw Glib::Error(1, 0, sqlite3_errmsg(new_handle));
	}
	this->~Connection();
	new (this) Connection(dbname, false);
}

void ORM::Connection::SQLExec0(const Glib::ustring& sql)
{
	char *err_msg = 0;
	std::clog << sql << std::endl;
	int sql_answer = sqlite3_exec(m_SQLite, sql.c_str(), ORM::Connection::CallBack0, this, &err_msg);
	if(sql_answer != SQLITE_OK)
	{
		Glib::ustring msg(err_msg);
		sqlite3_free(err_msg);
		throw Glib::Error(1, 0, msg);
	}
}

int ORM::Connection::CallBack0(void *self_ptr, int argc, char **argv, char **col_name)
{
	if(! self_ptr)
	{
		throw Glib::Error(1, 0, "Null this at callback.");
	}
	return 0;
}

void ORM::Connection::SQLExec(const Glib::ustring& sql, Glib::RefPtr<Data> &data)
{
	char *err_msg = 0;
	std::clog << sql << std::endl;
	data->clear();
	int sql_answer = sqlite3_exec(m_SQLite, sql.c_str(), ORM::Connection::CallBack, reinterpret_cast<void*>(DeRef(data)), &err_msg);
	if(sql_answer != SQLITE_OK)
	{
		Glib::ustring msg(err_msg);
		sqlite3_free(err_msg);
		throw Glib::Error(1, 0, msg);
	}
}

int ORM::Connection::CallBack(void *self_ptr, int argc, char **argv, char **col_name)
{
	if(! self_ptr)
	{
		throw Glib::Error(1, 0, "Null this at callback.");
	}
	Data *data = reinterpret_cast<Data*>(self_ptr);
	if(data->get_n_columns() != argc)
	{
		throw Glib::Error(1, 0, "Different size of data");
	}
	Gtk::TreeIter it = data->append();
	for(int index = 0; index < argc; index ++)
	{
		if(argv[index])
		{
			data->GetField(index).SetStrValue(it, argv[index]);
			std::clog << " [" << col_name[index] << "]=" << argv[index];
		}
		else
		{
			std::clog << " [" << col_name[index] << "]=NULL";
		}
	}
	std::clog << std::endl;
	return 0;
}

void ORM::Connection::SQLExecOwn(const Glib::ustring &sql, int (*callback)(void*,int,char**,char**), void* data)
{
	char *err_msg = 0;
	std::clog << sql << std::endl;
	int sql_answer = sqlite3_exec(m_SQLite, sql.c_str(), callback, data, &err_msg);
	if(sql_answer != SQLITE_OK)
	{
		Glib::ustring msg(err_msg);
		sqlite3_free(err_msg);
		throw Glib::Error(1, 0, msg);
	}
}

void ORM::Connection::Trace(void *, const char *sql)
{
	std::clog << "Trace: " << sql << std::endl;
}

