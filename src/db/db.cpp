#include <glibmm/error.h>
#include <iostream>
#include <cstdlib>
#include "db.h"
#include "models.h"

using namespace DB;

DataBase::DataBase(const Glib::ustring &db, bool create_new)
	:m_Connection(db, create_new)
{
#if 0
	int sql_answer;
	if(create_new)
	{
		sql_answer = sqlite3_open_v2(db.c_str(), &m_SQLite, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, 0);
	}
	else
	{
		sql_answer = sqlite3_open_v2(db.c_str(), &m_SQLite, SQLITE_OPEN_READWRITE, 0);
	}
	if(sql_answer != SQLITE_OK)
	{
		sqlite3_close(m_SQLite);
		throw Glib::Error(1, 0, sqlite3_errmsg(m_SQLite));
	}
#endif
	if(create_new)
	{
		// create tables
		CreateTableEntity(g_Auditoriums);
		CreateTableEntity(g_Days);
		CreateTableEntity(g_Hours);
		CreateTableEntity(g_Groups);
		CreateTableEntity(g_Teachers);
		CreateTableEntity(g_Lessons);
		CreateTableLinkN2N(g_LessonsAuditoriums);
		CreateTableLinkN2N(g_TeachersLessons);
		CreateTableLinkN2N(g_OrderLessons);
		CreateTableLinkTeachPlan(g_TeachPlan);
		CreateTableSchedule();
	}
}

#if 0
DataBase::~DataBase()
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
}
#endif

void DataBase::CreateTableEntity(const Entity& ent)
{
	m_Connection.SQLExec0(Glib::ustring::compose("DROP TABLE IF EXISTS %1", ent.m_TableName));
	if(&ent == &g_Auditoriums)
	{
		m_Connection.SQLExec0(Glib::ustring::compose("CREATE TABLE %1 (id INTEGER PRIMARY KEY AUTOINCREMENT, name VARCHAR(32) UNIQUE, multithr BOOL DEFAULT 0 )", ent.m_TableName));
	}
	else
	{
		m_Connection.SQLExec0(Glib::ustring::compose("CREATE TABLE %1 (id INTEGER PRIMARY KEY AUTOINCREMENT, name VARCHAR(32) UNIQUE)", ent.m_TableName));
	}
}

void DataBase::CreateTableLinkN2N(const Link_N2N& link)
{
	m_Connection.SQLExec0(Glib::ustring::compose("DROP TABLE IF EXISTS %1", link.m_TableName));
	m_Connection.SQLExec0(Glib::ustring::compose("CREATE TABLE %1 (id INTEGER PRIMARY KEY AUTOINCREMENT,	id_entity1 INTEGER NOT NULL REFERENCES %2(id) ON DELETE CASCADE ON UPDATE CASCADE, id_entity2 INTEGER NOT NULL REFERENCES %3(id) ON DELETE CASCADE ON UPDATE CASCADE)", link.m_TableName, link.m_Entity1.m_TableName, link.m_Entity2.m_TableName));
}

void DataBase::CreateTableLinkTeachPlan(const Link_TeachPlan& link)
{
	m_Connection.SQLExec0(Glib::ustring::compose("DROP TABLE IF EXISTS %1", link.m_TableName));
	m_Connection.SQLExec0(Glib::ustring::compose("CREATE TABLE %1 (id INTEGER PRIMARY KEY AUTOINCREMENT,	id_entity1 INTEGER NOT NULL REFERENCES %2(id) ON DELETE CASCADE ON UPDATE CASCADE, id_entity2 INTEGER NOT NULL REFERENCES %3(id) ON DELETE CASCADE ON UPDATE CASCADE, hours INTEGER NOT NULL DEFAULT 0)", link.m_TableName, link.m_Entity1.m_TableName, link.m_Entity2.m_TableName));
}

void DataBase::CreateTableSchedule()
{
	m_Connection.SQLExec0(Glib::ustring("DROP TABLE IF EXISTS Schedule"));
	m_Connection.SQLExec0(Glib::ustring::compose("CREATE TABLE Schedule (id_day INTEGER NOT NULL REFERENCES %1(id) ON DELETE CASCADE ON UPDATE CASCADE, id_hour INTEGER NOT NULL REFERENCES %2(id) ON DELETE CASCADE ON UPDATE CASCADE, id_record INTEGER NOT NULL REFERENCES %3(id) ON DELETE CASCADE ON UPDATE CASCADE, id_auditorium INTEGER NOT NULL REFERENCES %4(id) ON DELETE CASCADE ON UPDATE CASCADE)", g_Days.m_TableName, g_Hours.m_TableName, g_TeachPlan.m_TableName, g_Auditoriums.m_TableName));
}
#if 0
void DataBase::SQLExec0(const Glib::ustring& sql)
{
	char *err_msg = 0;
	std::clog << sql << std::endl;
	int sql_answer = sqlite3_exec(m_SQLite, sql.c_str(), DataBase::SQLCallBack0, this, &err_msg);
	if(sql_answer != SQLITE_OK)
	{
		Glib::ustring msg(err_msg);
		sqlite3_free(err_msg);
		throw Glib::Error(1, 0, msg);
	}
}

int DataBase::SQLCallBack0(void *self_ptr, int argc, char **argv, char **col_name)
{
	if(! self_ptr)
	{
		throw Glib::Error(1, 0, "Null this at callback.");
	}
	return 0;
}
#endif

void DataBase::AppendEntity(const Entity& ent, const Glib::ustring &name)
{
	m_Connection.SQLExec0(Glib::ustring::compose("INSERT INTO %1 (name) VALUES (\"%2\")", ent.m_TableName, name));
}

void DataBase::ListEntity(const Entity& ent, Glib::RefPtr<ORM::Table> &list_store)
{
	if(ent.m_SortByName)
	{
		m_Connection.SQLExec(Glib::ustring::compose("SELECT id, name FROM %1 ORDER BY name", ent.m_TableName), list_store);
	}
	else
	{
		m_Connection.SQLExec(Glib::ustring::compose("SELECT id, name FROM %1 ORDER BY id", ent.m_TableName), list_store);
	}
}

void DataBase::ListEntityAud(const Entity& ent, Glib::RefPtr<ORM::Table> &list_store)
{
	if(ent.m_SortByName)
	{
		m_Connection.SQLExec(Glib::ustring::compose("SELECT id, name, multithr FROM %1 ORDER BY name", ent.m_TableName), list_store);
	}
	else
	{
		m_Connection.SQLExec(Glib::ustring::compose("SELECT id, name, multithr FROM %1 ORDER BY id", ent.m_TableName), list_store);
	}
}

bool DataBase::GetAudMultithr(const Entity& ent, long id)
{
	bool res;
	SQLExec<bool>(Glib::ustring::compose("SELECT multithr FROM %1 WHERE id = %2", ent.m_TableName, id), &res);
	return res;
}

void DataBase::DeleteEntity(const Entity& ent, int id)
{
	m_Connection.SQLExec0(Glib::ustring::compose("DELETE FROM %1 WHERE id=%2", ent.m_TableName, id));
}

void DataBase::EditEntityName(const Entity& ent, int id, const Glib::ustring &new_name)
{
	m_Connection.SQLExec0(Glib::ustring::compose("UPDATE %1 SET name=\"%2\" WHERE id=%3", ent.m_TableName, new_name, id));
}

void DataBase::EditMultithr(const Entity& ent, int id, bool multithr)
{
	m_Connection.SQLExec0(Glib::ustring::compose("UPDATE %1 SET multithr=%2 WHERE id=%3", ent.m_TableName, multithr ? 1 : 0, id));
}

void DataBase::EditHours(const DB::Link_TeachPlan& link, int id, unsigned int hours)
{
	m_Connection.SQLExec0(Glib::ustring::compose("UPDATE %1 SET hours=%2 WHERE id=%3", link.m_TableName, hours, id));
}

void DataBase::ListLinkedEntity(const Link_N2N& link, int parent_id, Glib::RefPtr<ORM::Table> &list_store)
{
	m_Connection.SQLExec(Glib::ustring::compose("SELECT %2.id, %2.name FROM %1, %2 WHERE %1.id_entity1 = %3 AND %1.id_entity2 = %2.id", link.m_TableName, link.m_Entity2.m_TableName, parent_id), list_store);
}

void DataBase::AppendLinkedEntity(const Link_N2N& link, int parent_id, int child_id)
{
	m_Connection.SQLExec0(Glib::ustring::compose("INSERT INTO %1 (id_entity1, id_entity2) VALUES (%2, %3)", link.m_TableName, parent_id, child_id));
}

void DataBase::DeleteLinkedEntity(const Link_N2N& link, int parent_id, int child_id)
{
	m_Connection.SQLExec0(Glib::ustring::compose("DELETE FROM %1 WHERE id_entity1 = %2 AND id_entity2 = %3", link.m_TableName, parent_id, child_id));
}

void DataBase::ListLink(const Link_N2N& link, Glib::RefPtr<ORM::Table> &list_store)
{
	m_Connection.SQLExec(Glib::ustring::compose("SELECT %2.id, %2.name, _T.id, _T.name FROM %1, %2, %3 AS _T WHERE %1.id_entity1 = %2.id AND %1.id_entity2 = _T.id", link.m_TableName, link.m_Entity1.m_TableName, link.m_Entity2.m_TableName), list_store);
}

void DataBase::ListLinkedTeachPlan(const Link_TeachPlan& link, int parent_id, Glib::RefPtr<ORM::Table> &list_store)
{
	m_Connection.SQLExec(Glib::ustring::compose("SELECT %1.id, Lessons.name, Lessons.id, Teachers.id, Teachers.name, %1.hours FROM %1, %2, Lessons, Teachers WHERE %1.id_entity1 = %3 AND %1.id_entity2 = %2.id AND %2.id_entity1 = Teachers.id AND %2.id_entity2 = Lessons.id", link.m_TableName, link.m_Entity2.m_TableName, parent_id), list_store);
}

void DataBase::ListTeacherLessons(const Link_N2N& link, Glib::RefPtr<ORM::Table> &list_store)
{
	m_Connection.SQLExec(Glib::ustring::compose("SELECT %1.id, Lessons.name, Lessons.id, Teachers.id, Teachers.name FROM %1, Lessons, Teachers WHERE %1.id_entity1 = Teachers.id AND %1.id_entity2 = Lessons.id", link.m_TableName), list_store);
}

/*void DataBase::ListCircleLink(const Link_N2N& link, Glib::RefPtr<Gtk::ListStore> &list_store)
{
	SQLExec4isis(Glib::ustring::compose("SELECT %2.id, %2.name, _T.id, _T.name FROM OrderLessons, %2, Lessons AS _T WHERE OrderLessons.id_entity1 = Lessons.id AND OrderLessons.id_entity2 = _T.id", link.m_TableName, link.m_Entity1.m_TableName), list_store);
}*/

size_t DataBase::GetEntitiesCount(const DB::Entity& ent)
{
	size_t res;
	SQLExec<size_t>(Glib::ustring::compose("SELECT COUNT(*) FROM %1", ent.m_TableName), &res);
	return res;
}

size_t DataBase::GetEntitiesIDs(const DB::Entity& ent, std::vector<size_t> *array)
{
	return SQLExecArray<size_t>(Glib::ustring::compose("SELECT id FROM %1 ORDER BY id", ent.m_TableName), array);
}

bool DataBase::IsLinkBetween(const DB::Link_N2N &link, size_t id_a, size_t id_l)
{
	size_t res;
	SQLExec<size_t>(Glib::ustring::compose("SELECT COUNT(*) FROM %1 WHERE id_entity1 = %2 AND id_entity2 = %3", link.m_TableName, id_l, id_a), &res);
	return res > 0;
}

size_t DataBase::GetTForGL(size_t id_g, size_t id_l)
{
	size_t res;
	SQLExec<size_t>(Glib::ustring::compose("SELECT %1.id_entity1 FROM %1, %2 WHERE %1.id_entity2 = %4 AND %1.id = %2.id_entity2 AND %2.id_entity1 = %3", DB::g_TeachersLessons.m_TableName, DB::g_TeachPlan.m_TableName, id_g, id_l), &res);
	return res;
}

void DataBase::ListLessonRecords(Glib::RefPtr<ORM::Table> &list_store)
{
	m_Connection.SQLExec("SELECT GroupLessons.id, Teachers.id, Teachers.name, Lessons.id, Lessons.name, Groups.id, Groups.name, GroupLessons.hours FROM GroupLessons, Teachers, Lessons, Groups, TeachLes WHERE GroupLessons.id_entity1 = Groups.id AND GroupLessons.id_entity2 = TeachLes.id AND TeachLes.id_entity1 = Teachers.id AND TeachLes.id_entity2 = Lessons.id ORDER BY Groups.id, Teachers.id, Lessons.id", list_store);
}

void DataBase::GetGTList(Glib::RefPtr<ORM::Table> &list_store)
{
	m_Connection.SQLExec("SELECT DISTINCT Groups.id, Teachers.id FROM Groups, Teachers, GroupLessons, TeachLes WHERE GroupLessons.id_entity1 = Groups.id AND GroupLessons.id_entity2 = TeachLes.id AND TeachLes.id_entity1 = Teachers.id ORDER BY Groups.id, Teachers.id", list_store);
}

void DataBase::GetALList(Glib::RefPtr<ORM::Table> &list_store)
{
	m_Connection.SQLExec(Glib::ustring::compose("SELECT DISTINCT %1.id_entity1, %1.id_entity2 FROM %1 ORDER BY %1.id_entity1, %1.id_entity2", DB::g_LessonsAuditoriums.m_TableName), list_store);
}

