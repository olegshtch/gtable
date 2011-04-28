#include <glibmm/error.h>
#include <iostream>
#include <cstdlib>
#include "db.h"
#include "models.h"
#include "../orm/count.h"
#include "../orm/field.h"
#include "../orm/case.h"
#include "../orm/subquery.h"

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

void DataBase::New()
{
	m_Connection.~Connection();
	new (&m_Connection) ORM::Connection(":memory:", true);	
	ORM::Table::InitTables(m_Connection);
	m_Connection.InsertInto(g_ModelWeek);
	m_Connection.CreateTrigger("groupcat_trigger", "AFTER INSERT ON " + g_ModelGroups.GetTableName() + " BEGIN INSERT INTO " + g_ModelGroupCategory.GetTableName() + "(" + g_ModelGroupCategory.name.GetSmallFieldName() + "," + g_ModelGroupCategory.group.GetSmallFieldName() + "," + g_ModelGroupCategory.full.GetSmallFieldName() + ") VALUES (\"\", NEW." + g_ModelGroups.fId.GetSmallFieldName() + ", 1); END");
	m_Connection.CreateTrigger("subgroup_trigger", "AFTER INSERT ON " + g_ModelGroupCategory.GetTableName() + " BEGIN INSERT INTO " + g_ModelSubgroups.GetTableName() + "(" + g_ModelSubgroups.name.GetSmallFieldName() + "," + g_ModelSubgroups.group_category.GetSmallFieldName() + ") VALUES (\"\", NEW." + g_ModelGroupCategory.fId.GetSmallFieldName() + "); END");
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
		m_Connection.Update(ent)->Set(ent.GetField(i), row)->Where(ORM::Eq(ent.fId, row));
	}
}

Glib::ustring DataBase::GetTextById(const ORM::Table& ent, const ORM::Expr<Glib::ustring>& field, long id)
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

void DataBase::ListEntitiesText(const ORM::Table& ent, const ORM::Expr<Glib::ustring> field, Glib::RefPtr<ORM::Data> &data)
{
	m_Connection.Select(data, ORM::Expr<ORM::PrimaryKey>(ent.fId), field)->From(ent);
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
	m_Connection.Select(data, ORM::Count())->From(DB::g_ModelTeacherHolydays)->Where(ORM::Eq(DB::g_ModelTeacherHolydays.teacher, ORM::ForeignKey(teacher_id)) && ORM::Eq(DB::g_ModelTeacherHolydays.day, ORM::ForeignKey(day_id)) && ORM::Eq(DB::g_ModelTeacherHolydays.hour, ORM::ForeignKey(hour_id)));
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
		m_Connection.DeleteFrom(g_ModelTeacherHolydays)->Where(ORM::Eq(DB::g_ModelTeacherHolydays.teacher, ORM::ForeignKey(teacher_id)) && ORM::Eq(DB::g_ModelTeacherHolydays.day, ORM::ForeignKey(day_id)) && ORM::Eq(DB::g_ModelTeacherHolydays.hour, ORM::ForeignKey(hour_id)));
	}
}

bool DataBase::GetGroupHolydays(long int group_id, long int day_id, long int hour_id)
{
	ORM::Scheme scheme;
	ORM::Field<long int> long_field("");
	scheme.add(long_field);
	Glib::RefPtr<ORM::Data> data = ORM::Data::create(scheme);
	m_Connection.Select(data, ORM::Count())->From(DB::g_ModelGroupHolydays)->Where(ORM::Eq(DB::g_ModelGroupHolydays.group, ORM::ForeignKey(group_id)) && ORM::Eq(DB::g_ModelGroupHolydays.day, ORM::ForeignKey(day_id)) && ORM::Eq(DB::g_ModelGroupHolydays.hour, ORM::ForeignKey(hour_id)));
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
		m_Connection.DeleteFrom(g_ModelGroupHolydays)->Where(ORM::Eq(DB::g_ModelGroupHolydays.group, ORM::ForeignKey(group_id)) && ORM::Eq(DB::g_ModelGroupHolydays.day, ORM::ForeignKey(day_id)) && ORM::Eq(DB::g_ModelGroupHolydays.hour, ORM::ForeignKey(hour_id)));
	}
}

bool DataBase::GetWeeks()
{
	Glib::RefPtr<ORM::Data> data = ORM::Data::create(g_ModelWeek);
	m_Connection.Select(data)->From(g_ModelWeek);
	if(data->children().size())
	{
		return data->children()[0].get_value(g_ModelWeek.doubleweek);
	}
	return false;
}

void DataBase::SetWeeks(bool weeks)
{
	m_Connection.Update(g_ModelWeek)->Set(g_ModelWeek.doubleweek, weeks);
}

void DataBase::GetTeachingBranch(Glib::RefPtr<ORM::Data> &data, long int id_speciality)
{
	m_Connection.Select(data, g_ModelTeachingBranch.fId, g_ModelBranch.name)->From(g_ModelTeachingBranch, g_ModelBranch)->Where(ORM::Eq(g_ModelTeachingBranch.speciality, ORM::ForeignKey(id_speciality)) && ORM::Eq(g_ModelTeachingBranch.branch, g_ModelBranch.fId));
}

void DataBase::ListNewBranchForSpeciality(Glib::RefPtr<ORM::Data> &data, long int id_speciality)
{
	m_Connection.Select(data, g_ModelBranch.fId, g_ModelBranch.name)->From(g_ModelBranch);
}

void DataBase::AppendNewBranchForSpeciality(long int id_speciality, long int id_branch)
{
	Glib::RefPtr<ORM::Data> data = ORM::Data::create(g_ModelTeachingBranch);
	Gtk::TreeIter iter = data->append();
	iter->set_value(g_ModelTeachingBranch.branch, id_branch);
	iter->set_value(g_ModelTeachingBranch.speciality, id_speciality);
	m_Connection.InsertInto(g_ModelTeachingBranch, g_ModelTeachingBranch.branch, g_ModelTeachingBranch.speciality)->Values(iter);
}

long DataBase::GetTeachingPlanHours(long int id_teaching_branch, long int id_lesson_type)
{
	ORM::Scheme scheme;
	ORM::Field<long int> field("");
	scheme.add(field);
	Glib::RefPtr<ORM::Data> data = ORM::Data::create(scheme);
	m_Connection.Select(data, g_ModelTeachingPlan.hours)->From(g_ModelTeachingPlan, g_ModelTeachingBranch)->Where(ORM::Eq(g_ModelTeachingPlan.teaching_branch, ORM::ForeignKey(id_teaching_branch)) && ORM::Eq(g_ModelTeachingPlan.lesson_type, ORM::ForeignKey(id_lesson_type)));
	if(data->children().size() > 0)
	{
		return data->children()[0].get_value(field);
	}
	return 0;
}

void DataBase::EditTeachingPlanHours(long int id_teaching_branch, long int id_lesson_type, long hours)
{
	ORM::Scheme scheme;
	ORM::Field<long int> field("");
	scheme.add(field);
	Glib::RefPtr<ORM::Data> data = ORM::Data::create(scheme);
	m_Connection.Select(data, g_ModelTeachingPlan.hours)->From(g_ModelTeachingPlan, g_ModelTeachingBranch)->Where(ORM::Eq(g_ModelTeachingPlan.teaching_branch, ORM::ForeignKey(id_teaching_branch)) && ORM::Eq(g_ModelTeachingPlan.lesson_type, ORM::ForeignKey(id_lesson_type)));
	if(data->children().size() > 0)
	{
		m_Connection.Update(g_ModelTeachingPlan)->Set(g_ModelTeachingPlan.hours, hours)->Where(ORM::Eq(g_ModelTeachingPlan.teaching_branch, ORM::ForeignKey(id_teaching_branch)) && ORM::Eq(g_ModelTeachingPlan.lesson_type, ORM::ForeignKey(id_lesson_type)));
	}
	else
	{
		data = ORM::Data::create(g_ModelTeachingPlan);
		Gtk::TreeIter iter = data->append();
		iter->set_value(g_ModelTeachingPlan.teaching_branch, id_teaching_branch);
		iter->set_value(g_ModelTeachingPlan.lesson_type, id_lesson_type);
		iter->set_value(g_ModelTeachingPlan.hours, hours);
		m_Connection.InsertInto(g_ModelTeachingPlan)->Values(iter);
	}
}

void DataBase::GetSubgroupsList(Glib::RefPtr<ORM::Data>& data)
{
	ORM::Expr<Glib::ustring> name = ORM::Case<Glib::ustring>(ORM::Expr<bool>(g_ModelGroupCategory.full), g_ModelGroups.name).Else(ORM::Expr<Glib::ustring>(g_ModelGroups.name) + ":" + g_ModelGroupCategory.name + "#" + g_ModelSubgroups.name);
	m_Connection.Select(data, g_ModelSubgroups.fId, name)->From(g_ModelSubgroups, g_ModelGroupCategory, g_ModelGroups)->Where(ORM::Eq(g_ModelSubgroups.group_category, g_ModelGroupCategory.fId) && ORM::Eq(g_ModelGroupCategory.group, g_ModelGroups.fId));
}

void DataBase::GetLessonsForSubgroup(Glib::RefPtr<ORM::Data>& data, const ORM::ForeignKey& id_subgroup)
{
	//get speciality
	ORM::ForeignKey id_speciality = -1l;
	ORM::Scheme scheme;
	ORM::Field<long int> field("");
	scheme.add(field);
	Glib::RefPtr<ORM::Data> spec_data = ORM::Data::create(scheme);
	m_Connection.Select(spec_data, g_ModelGroups.speciality)->From(g_ModelGroups, g_ModelGroupCategory, g_ModelSubgroups)->Where(ORM::Eq(g_ModelSubgroups.fId, ORM::PrimaryKey(id_subgroup)) && ORM::Eq(g_ModelSubgroups.group_category, g_ModelGroupCategory.fId) && ORM::Eq(g_ModelGroupCategory.group, g_ModelGroups.fId));
	if(spec_data->children().size() > 0)
	{
		id_speciality = spec_data->children()[0].get_value(field);
	}

	ORM::Subquery lessons;
	lessons.Select(data, g_ModelTeachingPlan.fId, ORM::Expr<ORM::ForeignKey>(id_subgroup))->From(g_ModelTeachingPlan, g_ModelTeachingBranch)->Where(ORM::Eq(g_ModelTeachingPlan.teaching_branch, g_ModelTeachingBranch.fId) && ORM::Eq(g_ModelTeachingBranch.speciality, id_speciality) && ORM::Greater(g_ModelTeachingPlan.hours, 0l));
	m_Connection.InsertInto(g_ModelLessons, g_ModelLessons.teaching_plan, g_ModelLessons.subgroup)->Select(lessons);

	ORM::Expr<Glib::ustring> name = ORM::Expr<Glib::ustring>(g_ModelBranch.name) + "\\" + g_ModelLessonType.name;
	m_Connection.Select(data, g_ModelLessons.fId, name, g_ModelLessons.teacher)->From(g_ModelLessons, g_ModelTeachingPlan, g_ModelBranch, g_ModelLessonType, g_ModelTeachingBranch)->Where(ORM::Eq(g_ModelLessons.subgroup, id_subgroup) && ORM::Eq(g_ModelLessons.teaching_plan, g_ModelTeachingPlan.fId) && ORM::Eq(g_ModelTeachingPlan.teaching_branch, g_ModelTeachingBranch.fId) && ORM::Eq(g_ModelTeachingPlan.lesson_type, g_ModelLessonType.fId) && ORM::Eq(g_ModelTeachingBranch.branch, g_ModelBranch.fId) && ORM::Greater(g_ModelTeachingPlan.hours, 0l));
}

void DataBase::SetLessonsTeacher(ORM::PrimaryKey id_lesson, ORM::ForeignKey id_teacher)
{
	m_Connection.Update(g_ModelLessons)->Set(g_ModelLessons.teacher, id_teacher)->Where(ORM::Eq(g_ModelLessons.fId, id_lesson));
}

void DataBase::ListGroupOtherLessons(long int id_group, Glib::RefPtr<ORM::Data>& data)
{
	data->clear();

	ORM::Scheme scheme;
	ORM::Field<ORM::PrimaryKey> id(g_ModelLessons);
	//ORM::Field<Glib::ustring> auditory_name("auditory");
	ORM::Field<Glib::ustring> teacher_name("teacher");
	ORM::Field<Glib::ustring> lesson_name("lesson");
	scheme.add(id);
	//scheme.add(auditory_name);
	scheme.add(teacher_name);
	scheme.add(lesson_name);
	Glib::RefPtr<ORM::Data> lesson_list = ORM::Data::create(scheme);

	m_Connection.Select(lesson_list, g_ModelLessons.fId, ORM::Expr<Glib::ustring>(DB::g_ModelTeachers.secondname) + " " + DB::g_ModelTeachers.firstname + " " + DB::g_ModelTeachers.thirdname, ORM::Expr<Glib::ustring>(DB::g_ModelBranch.name) + "\\" + DB::g_ModelLessonType.name)->From(g_ModelLessons, g_ModelTeachers, g_ModelBranch, g_ModelLessonType);
}

