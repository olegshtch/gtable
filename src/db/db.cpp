#include <glibmm/error.h>
#include <iostream>
#include <cstdlib>
#include "db.h"
#include "models.h"
#include "../orm/count.h"
#include "../orm/field.h"

using namespace DB;

DataBase* DataBase::s_Ptr = new DataBase(":memory:");

void DataBase::Free()
{
	if(s_Ptr)
	{
		delete s_Ptr;
		s_Ptr = 0;
	}
}

DataBase::DataBase(const Glib::ustring &db)
	:m_Connection(db, true)
{
	atexit(Free);
}

void DataBase::AppendEntity(const ORM::Table& ent, const Gtk::TreeIter& row)
{
	m_Connection.InsertInto(ent)->Values(row);
}

void DataBase::ListEntity(const ORM::Table& ent,Glib::RefPtr<ORM::Data> &list_store)
{
	m_Connection.Select(list_store)->From(ent);
}

void DataBase::EditEntity(const ORM::Table& ent, const Gtk::TreeIter& row)
{
	for(size_t i = 1; i < ent.size(); ++ i)
	{
		m_Connection.Update(ent)->Set(ent.GetField(i), row).Where(ORM::Eq(ent.fId, row));
	}
}

Glib::ustring DataBase::GetTextById(const ORM::Table& ent, const ORM::Field<Glib::ustring>& field, long id)
{
	ORM::Scheme scheme;
	ORM::Field<Glib::ustring> str_field("");
	scheme.add(str_field);
	Glib::RefPtr<ORM::Data> data = ORM::Data::create(scheme);
	m_Connection.Select(data, field)->From(ent)->Where(ORM::Eq(ent.fId, static_cast<ORM::PrimaryKey>(id)));
	if(data->children().size())
	{
		return data->children().begin()->get_value(str_field);
	}
	return "{null}";
}

void DataBase::ListEntitiesText(const ORM::Table& ent, const ORM::Field<Glib::ustring> field, Glib::RefPtr<ORM::Data> &data)
{
	m_Connection.Select(data, ent.fId, field)->From(ent);
}

void DataBase::RemoveEntity(const ORM::Table& ent, const Gtk::TreeIter& row)
{
	m_Connection.DeleteFrom(ent)->Where(ORM::Eq(ent.fId, row));
}

bool DataBase::GetTeacherHolydays(long int teacher_id, long int day_id, long int hour_id)
{
	ORM::Scheme scheme;
	ORM::Field<long int> long_field("");
	scheme.add(long_field);
	Glib::RefPtr<ORM::Data> data = ORM::Data::create(scheme);
	const ORM::FieldBase& field = ORM::Count();
	m_Connection.Select(data, field)->From(DB::g_ModelTeacherHolydays)->Where(ORM::Eq(DB::g_ModelTeacherHolydays.teacher, ORM::ForeignKey(teacher_id)) and ORM::Eq(DB::g_ModelTeacherHolydays.day, ORM::ForeignKey(day_id)) and ORM::Eq(DB::g_ModelTeacherHolydays.hour, ORM::ForeignKey(hour_id)));
	if(data->children().size())
	{
		return static_cast<bool>(data->children()[0].get_value(long_field));
	}
	return false;
}

void DataBase::SetTeacherHolydays(long int teacher_id, long int day_id, long int hour_id, bool holyday)
{
	if(holyday)
	{
		Glib::RefPtr<ORM::Data> data = ORM::Data::create(g_ModelTeacherHolydays);
		Gtk::TreeIter iter = data->append();
		iter->set_value(g_ModelTeacherHolydays.teacher, teacher_id);
		iter->set_value(g_ModelTeacherHolydays.day, day_id);
		iter->set_value(g_ModelTeacherHolydays.hour, hour_id);
		m_Connection.InsertInto(g_ModelTeacherHolydays)->Values(iter);
	}
	else
	{
		m_Connection.DeleteFrom(g_ModelTeacherHolydays)->Where(ORM::Eq(DB::g_ModelTeacherHolydays.teacher, ORM::ForeignKey(teacher_id)) and ORM::Eq(DB::g_ModelTeacherHolydays.day, ORM::ForeignKey(day_id)) and ORM::Eq(DB::g_ModelTeacherHolydays.hour, ORM::ForeignKey(hour_id)));
	}
}

bool DataBase::GetGroupHolydays(long int group_id, long int day_id, long int hour_id)
{
	ORM::Scheme scheme;
	ORM::Field<long int> long_field("");
	scheme.add(long_field);
	Glib::RefPtr<ORM::Data> data = ORM::Data::create(scheme);
	const ORM::FieldBase& field = ORM::Count();
	m_Connection.Select(data, field)->From(DB::g_ModelGroupHolydays)->Where(ORM::Eq(DB::g_ModelGroupHolydays.group, ORM::ForeignKey(group_id)) and ORM::Eq(DB::g_ModelGroupHolydays.day, ORM::ForeignKey(day_id)) and ORM::Eq(DB::g_ModelGroupHolydays.hour, ORM::ForeignKey(hour_id)));
	if(data->children().size())
	{
		return static_cast<bool>(data->children()[0].get_value(long_field));
	}
	return false;
}

void DataBase::SetGroupHolydays(long int group_id, long int day_id, long int hour_id, bool holyday)
{
	if(holyday)
	{
		Glib::RefPtr<ORM::Data> data = ORM::Data::create(g_ModelGroupHolydays);
		Gtk::TreeIter iter = data->append();
		iter->set_value(g_ModelGroupHolydays.group, group_id);
		iter->set_value(g_ModelGroupHolydays.day, day_id);
		iter->set_value(g_ModelGroupHolydays.hour, hour_id);
		m_Connection.InsertInto(g_ModelGroupHolydays)->Values(iter);
	}
	else
	{
		m_Connection.DeleteFrom(g_ModelGroupHolydays)->Where(ORM::Eq(DB::g_ModelGroupHolydays.group, ORM::ForeignKey(group_id)) and ORM::Eq(DB::g_ModelGroupHolydays.day, ORM::ForeignKey(day_id)) and ORM::Eq(DB::g_ModelGroupHolydays.hour, ORM::ForeignKey(hour_id)));
	}
}

