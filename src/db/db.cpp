#include <glibmm/error.h>
#include <iostream>
#include <cstdlib>
#include "db.h"
#include "models.h"

using namespace DB;

DataBase* DataBase::s_Ptr = new DataBase(":memory:");

void DataBase::Free()
{
	delete s_Ptr;
}

DataBase::DataBase(const Glib::ustring &db)
	:m_Connection(db, true)
{
	atexit(Free);
	InitTable();
}

void DataBase::InitTable()
{
	m_Connection.CreateTable(g_ModelAud);
	m_Connection.CreateTable(g_ModelDays);
	m_Connection.CreateTable(g_ModelHours);
	m_Connection.CreateTable(g_ModelGroups);
	m_Connection.CreateTable(g_ModelTeachers);
	m_Connection.CreateTable(g_ModelLessons);
}

void DataBase::AppendEntity(const ModelEntity& ent, const Glib::ustring &name)
{
	m_Connection.SQLExec0(Glib::ustring::compose("INSERT INTO %1 (name) VALUES (\"%2\")", ent.GetTableName(), name));
}

void DataBase::ListEntity(const ORM::Table& ent,Glib::RefPtr<ORM::Data> &list_store, bool sort_by_name)
{
	m_Connection.Select(list_store)->From(ent);
}

bool DataBase::GetAudMultithr(const ModelEntity& ent, long id)
{
	bool res;
	SQLExec<bool>(Glib::ustring::compose("SELECT multithr FROM %1 WHERE id = %2", ent.GetTableName(), id), &res);
	return res;
}

void DataBase::DeleteEntity(const ModelEntity& ent, int id)
{
	m_Connection.SQLExec0(Glib::ustring::compose("DELETE FROM %1 WHERE id=%2", ent.GetTableName(), id));
}

void DataBase::EditEntityName(const ModelEntity& ent, int id, const Glib::ustring &new_name)
{
	m_Connection.SQLExec0(Glib::ustring::compose("UPDATE %1 SET name=\"%2\" WHERE id=%3", ent.GetTableName(), new_name, id));
}

void DataBase::EditMultithr(const ModelEntity& ent, int id, bool multithr)
{
	m_Connection.SQLExec0(Glib::ustring::compose("UPDATE %1 SET multithr=%2 WHERE id=%3", ent.GetTableName(), multithr ? 1 : 0, id));
}
#if 0
void DataBase::EditHours(const DB::Link_TeachPlan& link, int id, unsigned int hours)
{
	m_Connection.SQLExec0(Glib::ustring::compose("UPDATE %1 SET hours=%2 WHERE id=%3", link.m_TableName, hours, id));
}

void DataBase::ListLinkedEntity(const Link_N2N& link, int parent_id, Glib::RefPtr<ORM::Data> &list_store)
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

void DataBase::ListLink(const Link_N2N& link, Glib::RefPtr<ORM::Data> &list_store)
{
	m_Connection.SQLExec(Glib::ustring::compose("SELECT %2.id, %2.name, _T.id, _T.name FROM %1, %2, %3 AS _T WHERE %1.id_entity1 = %2.id AND %1.id_entity2 = _T.id", link.m_TableName, link.m_Entity1.m_TableName, link.m_Entity2.m_TableName), list_store);
}

void DataBase::ListLinkedTeachPlan(const Link_TeachPlan& link, int parent_id, Glib::RefPtr<ORM::Data> &list_store)
{
	m_Connection.SQLExec(Glib::ustring::compose("SELECT %1.id, Lessons.name, Lessons.id, Teachers.id, Teachers.name, %1.hours FROM %1, %2, Lessons, Teachers WHERE %1.id_entity1 = %3 AND %1.id_entity2 = %2.id AND %2.id_entity1 = Teachers.id AND %2.id_entity2 = Lessons.id", link.m_TableName, link.m_Entity2.m_TableName, parent_id), list_store);
}

void DataBase::ListTeacherLessons(const Link_N2N& link, Glib::RefPtr<ORM::Data> &list_store)
{
	m_Connection.SQLExec(Glib::ustring::compose("SELECT %1.id, Lessons.name, Lessons.id, Teachers.id, Teachers.name FROM %1, Lessons, Teachers WHERE %1.id_entity1 = Teachers.id AND %1.id_entity2 = Lessons.id", link.m_TableName), list_store);
}

void DataBase::ListCircleLink(const Link_N2N& link, Glib::RefPtr<Gtk::ListStore> &list_store)
{
	SQLExec4isis(Glib::ustring::compose("SELECT %2.id, %2.name, _T.id, _T.name FROM OrderLessons, %2, Lessons AS _T WHERE OrderLessons.id_entity1 = Lessons.id AND OrderLessons.id_entity2 = _T.id", link.m_TableName, link.m_Entity1.m_TableName), list_store);
}
#endif

size_t DataBase::GetEntitiesCount(const DB::ModelEntity& ent)
{
	size_t res;
	SQLExec<size_t>(Glib::ustring::compose("SELECT COUNT(*) FROM %1", ent.GetTableName()), &res);
	return res;
}

size_t DataBase::GetEntitiesIDs(const DB::ModelEntity& ent, std::vector<size_t> *array)
{
	return SQLExecArray<size_t>(Glib::ustring::compose("SELECT id FROM %1 ORDER BY id", ent.GetTableName()), array);
}

Glib::ustring DataBase::GetEntityName(const ModelEntity& ent, int id)
{
	Glib::ustring res;
	SQLExecString(Glib::ustring::compose("SELECT name FROM %1 WHERE id = %2", ent.GetTableName(), id), &res);
	return res;
}

#if 0
bool DataBase::IsLinkBetween(const DB::Link_N2N &link, size_t id_a, size_t id_l)
{
	size_t res;
	SQLExec<size_t>(Glib::ustring::compose("SELECT COUNT(*) FROM %1 WHERE id_entity1 = %2 AND id_entity2 = %3", link.m_TableName, id_l, id_a), &res);
	return res > 0;
}

size_t DataBase::GetTForGL(size_t id_g, size_t id_l)
{
	size_t res;
	SQLExec<size_t>(Glib::ustring::compose("SELECT %1.id_entity1 FROM %1, %2 WHERE %1.id_entity2 = %4 AND %1.id = %2.id_entity2 AND %2.id_entity1 = %3", DB::g_TeachersLessons.GetTableName(), DB::g_TeachPlan.GetTableName(), id_g, id_l), &res);
	return res;
}
#endif

void DataBase::ListLessonRecords(Glib::RefPtr<ORM::Data> &list_store)
{
	m_Connection.SQLExec("SELECT GroupLessons.id, Teachers.id, Teachers.name, Lessons.id, Lessons.name, Groups.id, Groups.name, GroupLessons.hours FROM GroupLessons, Teachers, Lessons, Groups, TeachLes WHERE GroupLessons.id_entity1 = Groups.id AND GroupLessons.id_entity2 = TeachLes.id AND TeachLes.id_entity1 = Teachers.id AND TeachLes.id_entity2 = Lessons.id ORDER BY Groups.id, Teachers.id, Lessons.id", list_store);
}

void DataBase::GetGTList(Glib::RefPtr<ORM::Data> &list_store)
{
	m_Connection.SQLExec("SELECT DISTINCT Groups.id, Teachers.id FROM Groups, Teachers, GroupLessons, TeachLes WHERE GroupLessons.id_entity1 = Groups.id AND GroupLessons.id_entity2 = TeachLes.id AND TeachLes.id_entity1 = Teachers.id ORDER BY Groups.id, Teachers.id", list_store);
}

#if 0
void DataBase::GetALList(Glib::RefPtr<ORM::Data> &list_store)
{
	m_Connection.SQLExec(Glib::ustring::compose("SELECT DISTINCT %1.id_entity1, %1.id_entity2 FROM %1 ORDER BY %1.id_entity1, %1.id_entity2", DB::g_LessonsAuditoriums.m_TableName), list_store);
}
#endif

