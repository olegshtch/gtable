#include <glibmm/error.h>
#include <iostream>
#include <cstdlib>
#include "db.h"
#include "models.h"
#include "id_text_scheme.h"
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
	m_Connection.CreateTrigger("groupcat_trigger", "AFTER INSERT ON " + g_ModelGroups.GetTableName() + " BEGIN INSERT OR FAIL INTO " + g_ModelGroupCategory.GetTableName() + "(" + g_ModelGroupCategory.name.GetSmallFieldName() + "," + g_ModelGroupCategory.group.GetSmallFieldName() + "," + g_ModelGroupCategory.full.GetSmallFieldName() + ") VALUES (\"\", NEW." + g_ModelGroups.fId.GetSmallFieldName() + ", 1); END");
	m_Connection.CreateTrigger("subgroup_trigger", "AFTER INSERT ON " + g_ModelGroupCategory.GetTableName() + " BEGIN INSERT OR FAIL INTO " + g_ModelSubgroups.GetTableName() + "(" + g_ModelSubgroups.name.GetSmallFieldName() + "," + g_ModelSubgroups.group_category.GetSmallFieldName() + ") VALUES (\"\", NEW." + g_ModelGroupCategory.fId.GetSmallFieldName() + "); END");
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

void DataBase::ListIDs(const ORM::Table& ent, Glib::RefPtr<ORM::Data> &data)
{
	m_Connection.Select(data, ent.fId)->From(ent);
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
	iter->set_value(g_ModelTeachingBranch.term, 0l); //todo: fix it!
	m_Connection.InsertInto(g_ModelTeachingBranch
		, g_ModelTeachingBranch.branch
		, g_ModelTeachingBranch.speciality
		, g_ModelTeachingBranch.term)
		->Values(iter);
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
	std::cout << "DataBase::GetLessonsForSubgroup id_subgroup=" << id_subgroup << std::endl;
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

	//Get lessons without stream
	ORM::Subquery tp; // list teaching_plan in LessonSubgroup for subgroup
	tp.Select(data
		, g_ModelLessonSubgroup.teaching_plan)
		->From(g_ModelLessonSubgroup)
		->NaturalJoin(g_ModelTeachingPlan)
		->Where(ORM::Eq(g_ModelLessonSubgroup.subgroup, id_subgroup) && ORM::Greater(g_ModelTeachingPlan.hours, 0l));

	ORM::Scheme scheme_tp;
	ORM::Field<long int> id("");
	ORM::Field<long int> branch_id("");
	ORM::Field<long int> lesson_id("");
	scheme_tp.add(id);
	scheme_tp.add(branch_id);
	scheme_tp.add(lesson_id);

	Glib::RefPtr<ORM::Data> adding_tp = ORM::Data::create(scheme_tp); // list of teaching_plan for adding

	m_Connection.Select(adding_tp
		, g_ModelTeachingPlan.fId
		, g_ModelTeachingBranch.branch
		, g_ModelTeachingPlan.lesson_type)
		->From(g_ModelTeachingPlan)
		->NaturalJoin(g_ModelTeachingBranch)
		->Where(ORM::Eq(g_ModelTeachingBranch.speciality, id_speciality) && ORM::Greater(g_ModelTeachingPlan.hours, 0l) && ORM::NotIn(g_ModelTeachingPlan.fId, tp));

	if(adding_tp->children().size() > 0)
	{
		// add new Lessons and LessonSubgroup
		for(Gtk::TreeIter iter = adding_tp->children().begin(); iter != adding_tp->children().end(); ++ iter)
		{
			long int branch = iter->get_value(branch_id);
			long int lesson_type = iter->get_value(lesson_id);

			Glib::RefPtr<ORM::Data> lesson_data = ORM::Data::create(g_ModelLessons);
			Gtk::TreeIter lesson_iter = lesson_data->append();
			lesson_iter->set_value(g_ModelLessons.branch, branch);
			lesson_iter->set_value(g_ModelLessons.lesson_type, lesson_type);
			m_Connection.InsertInto(g_ModelLessons, g_ModelLessons.branch, g_ModelLessons.lesson_type)->Values(lesson_iter);
		}

		ORM::Subquery lesson_with_subgroup;
		lesson_with_subgroup.Select(data
			, g_ModelLessonSubgroup.lesson)
			->From(g_ModelLessonSubgroup);

		ORM::Subquery adding_lesson_subgroup;
		adding_lesson_subgroup.Select(data
			, g_ModelLessons.fId
			, ORM::Expr<ORM::ForeignKey>(id_subgroup)
			, g_ModelTeachingPlan.fId)
			->From(g_ModelLessons)
			->NaturalJoin(g_ModelBranch)
			->NaturalJoin(g_ModelLessonType)
			->NaturalJoin(g_ModelTeachingPlan)
			->NaturalJoin(g_ModelTeachingBranch)
			->Where(ORM::Greater(g_ModelTeachingPlan.hours, 0l) && ORM::NotIn(g_ModelLessons.fId, lesson_with_subgroup) && ORM::Eq(g_ModelTeachingBranch.speciality, id_speciality));

		m_Connection.InsertInto(g_ModelLessonSubgroup
			, g_ModelLessonSubgroup.lesson
			, g_ModelLessonSubgroup.subgroup
			, g_ModelLessonSubgroup.teaching_plan)
			->Select(adding_lesson_subgroup);
	}

	ORM::Expr<Glib::ustring> name = ORM::Expr<Glib::ustring>(g_ModelBranch.name) + "\\" + g_ModelLessonType.name;
	m_Connection.Select(data
		, g_ModelLessons.fId
		, name
		//, ORM::group_concat(g_ModelGroups.name)
		, g_ModelLessons.teacher)
		->From(g_ModelLessons)
		->NaturalJoin(g_ModelTeachingPlan)
		->NaturalJoin(g_ModelBranch)
		->NaturalJoin(g_ModelLessonType)
		->NaturalJoin(g_ModelTeachingBranch)
		->NaturalJoin(g_ModelSubgroups)
		->NaturalJoin(g_ModelGroupCategory)
		->NaturalJoin(g_ModelSpecialities)
		->NaturalJoin(g_ModelGroups)
		->NaturalJoin(g_ModelLessonSubgroup)
		->Where(ORM::Eq(g_ModelTeachingPlan.teaching_branch, g_ModelTeachingBranch.fId) && ORM::Eq(g_ModelTeachingPlan.lesson_type, g_ModelLessonType.fId) && ORM::Eq(g_ModelTeachingBranch.branch, g_ModelBranch.fId) && ORM::Greater(g_ModelTeachingPlan.hours, 0l)
			&& ORM::Eq(g_ModelSubgroups.fId, ORM::PrimaryKey(id_subgroup)));
	std::cout << "DataBase::GetLessonsForSubgroup END" << id_subgroup << std::endl;
}

void DataBase::SetLessonTeacher(ORM::PrimaryKey id_lesson, ORM::ForeignKey id_teacher)
{
	m_Connection.Update(g_ModelLessons)->Set(g_ModelLessons.teacher, id_teacher)->Where(ORM::Eq(g_ModelLessons.fId, id_lesson));
}

void DataBase::GetLessonsListForAdding(Glib::RefPtr<ORM::Data>& data, const ORM::ForeignKey& id_subgroup, const ORM::PrimaryKey& id_lesson)
{
	long int id_branch = -1;
	long int id_lesson_type = -1;
	long int lesson_hours = -1;
	ORM::Scheme scheme;
	ORM::Field<ORM::ForeignKey> f_branch(g_ModelBranch);
	ORM::Field<ORM::ForeignKey> f_lesson_type(g_ModelLessonType);
	ORM::Field<long int> f_hours("");
	scheme.add(f_branch);
	scheme.add(f_lesson_type);
	scheme.add(f_hours);
	Glib::RefPtr<ORM::Data> lesson_data = ORM::Data::create(scheme);
	m_Connection.SelectDistinct(lesson_data
		, g_ModelLessons.branch
		, g_ModelLessons.lesson_type
		, g_ModelTeachingPlan.hours)
		->From(g_ModelLessons)
		->NaturalJoin(g_ModelLessonSubgroup)
		->NaturalJoin(g_ModelTeachingPlan)
		->NaturalJoin(g_ModelTeachingBranch)
		->NaturalJoin(g_ModelLessonType)
		->Where(ORM::Eq(g_ModelLessons.fId, id_lesson)
			&& ORM::Eq(g_ModelLessonType.multithread, true));
	
	if(lesson_data->children().size() == 1)
	{
		id_branch = lesson_data->children()[0].get_value(f_branch);
		id_lesson_type = lesson_data->children()[0].get_value(f_lesson_type);
		lesson_hours = lesson_data->children()[0].get_value(f_hours);

		//get lessons with same parameters without this subgroup
		m_Connection.Select(data
			, g_ModelLessons.fId
			, ORM::group_concat(g_ModelGroups.name))
			->From(g_ModelLessons)
			->NaturalJoin(g_ModelLessonSubgroup)
			->NaturalJoin(g_ModelSubgroups)
			->NaturalJoin(g_ModelGroupCategory)
			->NaturalJoin(g_ModelGroups)
			->NaturalJoin(g_ModelSpecialities)
			->NaturalJoin(g_ModelTeachingBranch)
			->NaturalJoin(g_ModelTeachingPlan)
			->Where(ORM::NonEq(g_ModelLessons.fId, id_lesson)
				&& ORM::Eq(g_ModelLessons.branch, ORM::ForeignKey(id_branch))
				&& ORM::Eq(g_ModelLessons.lesson_type, ORM::ForeignKey(id_lesson_type))
				&& ORM::Eq(g_ModelTeachingPlan.hours, lesson_hours))
			->GroupBy(g_ModelLessons.fId);
	}
	else if(lesson_data->children().size() > 1)
	{
		std::cout << "Wrong count streams" << std::endl;
	}
}

void DataBase::ListGroupOtherLessons(long int id_group, Glib::RefPtr<ORM::Data>& data)
{
	data->clear();

	ORM::Scheme scheme;
	ORM::Field<ORM::PrimaryKey> id;
	//ORM::Field<Glib::ustring> auditory_name("auditory");
	ORM::Field<Glib::ustring> teacher_name("teacher");
	ORM::Field<Glib::ustring> lesson_name("lesson");
	ORM::Field<long int> hours("hours");
	scheme.add(id);
	//scheme.add(auditory_name);
	scheme.add(teacher_name);
	scheme.add(lesson_name);
	scheme.add(hours);
	Glib::RefPtr<ORM::Data> lesson_list = ORM::Data::create(scheme);

	m_Connection.Select(lesson_list
		, g_ModelLessons.fId
		, ORM::Expr<Glib::ustring>(DB::g_ModelTeachers.secondname) + " " + ORM::substr(DB::g_ModelTeachers.firstname, 1, 1) + ". " + ORM::substr(DB::g_ModelTeachers.thirdname, 1, 1) + "."
		, ORM::Expr<Glib::ustring>(DB::g_ModelBranch.name) + "\\" + DB::g_ModelLessonType.name
		, g_ModelTeachingPlan.hours)
		->From(g_ModelLessons, g_ModelGroupCategory, g_ModelLessonSubgroup)
		->NaturalJoin(g_ModelTeachingPlan)
		->NaturalJoin(g_ModelLessonType)
		->NaturalJoin(g_ModelBranch)
		->NaturalJoin(g_ModelTeachingBranch)
		->NaturalJoin(g_ModelTeachers)
		->NaturalJoin(g_ModelSubgroups)
		->Where(ORM::Greater(g_ModelTeachingPlan.hours, 0L)
			&& ORM::Eq(g_ModelGroupCategory.group, ORM::ForeignKey(id_group))
			&& ORM::Eq(g_ModelLessons.fId, g_ModelLessonSubgroup.lesson));

	for(Gtk::TreeIter it = lesson_list->children().begin(); it != lesson_list->children().end(); ++ it)
	{
		// get count lesson in timetable
		long int lesson_count = 0;
		ORM::Scheme count_scheme;
		ORM::Field<long int> count("");
		count_scheme.add(count);
		Glib::RefPtr<ORM::Data> count_data = ORM::Data::create(count_scheme);
		m_Connection.Select(count_data
			, ORM::Count())
			->From(g_ModelSchedule)
			->Where(ORM::Eq(g_ModelSchedule.lesson, ORM::ForeignKey(it->get_value(id))));
		if(count_data->children().size() > 0)
		{
			lesson_count = count_data->children()[0].get_value(count);
		}

		if(lesson_count > it->get_value(hours))
		{
			std::cout << "Too more lessons=" << it->get_value(id) << " " << it->get_value(lesson_name) << std::endl;
		}

		for(long h = it->get_value(hours) - lesson_count; h > 0; -- h)
		{
			Gtk::TreeIter iter = data->append();
			iter->set_value(g_IdTextScheme.fId, it->get_value(id));
			iter->set_value(g_IdTextScheme.fText, it->get_value(lesson_name) + "\n" + it->get_value(teacher_name));
		}
	}
}

void DataBase::GetAuditoriumListForLesson(Glib::RefPtr<ORM::Data>& data, ORM::PrimaryKey lesson_id, ORM::ForeignKey day_id, ORM::ForeignKey hour_id)
{
	//subquery for busy auditorium
	ORM::Subquery subquery_busy;
	subquery_busy.Select(data
		, g_ModelSchedule.auditorium)
		->From(g_ModelSchedule)
		->Where(ORM::Eq(g_ModelSchedule.day, day_id) && ORM::Eq(g_ModelSchedule.hour, hour_id));

	//get all acceptable auditorium
	m_Connection.SelectDistinct(data
		, g_ModelAuditoriums.fId
		, g_ModelAuditoriums.name)
		->From(g_ModelAuditoriums, g_ModelLessons)
		->NaturalJoin(g_ModelTeachingPlan)
		->NaturalJoin(g_ModelLessonType)
		->NaturalJoin(g_ModelTeachingBranch)
		->NaturalJoin(g_ModelLessonSubgroup)
		->Where(ORM::NotIn(g_ModelAuditoriums.fId, subquery_busy) && ORM::Eq(g_ModelLessons.fId, lesson_id)
			&& ORM::Eq(g_ModelAuditoriums.multithread, g_ModelLessonType.multithread));
}

void DataBase::SetLessonIntoTimetable(long int id_lesson, long int id_aud, long int id_day, long int id_hour)
{
	std::cout << "DataBase::SetLessonIntoTimetable" << std::endl;
	Glib::RefPtr<ORM::Data> data = ORM::Data::create(g_ModelSchedule);
	Gtk::TreeIter iter = data->append();
	iter->set_value(g_ModelSchedule.auditorium, id_aud);
	iter->set_value(g_ModelSchedule.day, id_day);
	iter->set_value(g_ModelSchedule.hour, id_hour);
	iter->set_value(g_ModelSchedule.lesson, id_lesson);
	if(iter)
	{
		m_Connection.InsertInto(g_ModelSchedule)->Values(iter);
	}
}

void DataBase::RemoveLessonFromTimetable(long int id_lesson, long int id_day, long int id_hour)
{
	m_Connection.DeleteFrom(g_ModelSchedule)
		->Where(ORM::Eq(g_ModelSchedule.lesson, ORM::ForeignKey(id_lesson))
			&& ORM::Eq(g_ModelSchedule.day, ORM::ForeignKey(id_day))
			&& ORM::Eq(g_ModelSchedule.hour, ORM::ForeignKey(id_hour)));
}

Glib::ustring DataBase::GetTimeTableLessonGroupText(ORM::ForeignKey id_group, ORM::ForeignKey id_hour, ORM::ForeignKey id_day)
{
	ORM::Scheme scheme;
	ORM::Field<ORM::PrimaryKey> id;
	ORM::Field<Glib::ustring> auditory_name("auditory");
	ORM::Field<Glib::ustring> teacher_name("teacher");
	ORM::Field<Glib::ustring> lesson_name("lesson");
	scheme.add(id);
	scheme.add(auditory_name);
	scheme.add(teacher_name);
	scheme.add(lesson_name);
	Glib::RefPtr<ORM::Data> data = ORM::Data::create(scheme);

	m_Connection.Select(data
		, g_ModelSchedule.fId
		, g_ModelAuditoriums.name
		, ORM::Expr<Glib::ustring>(DB::g_ModelTeachers.secondname) + " " + ORM::substr(DB::g_ModelTeachers.firstname, 1, 1) + ". " + ORM::substr(DB::g_ModelTeachers.thirdname, 1, 1) + "."
		, ORM::Expr<Glib::ustring>(DB::g_ModelBranch.name) + "\\" + DB::g_ModelLessonType.name)
		->From(g_ModelSchedule, g_ModelSpecialities, g_ModelTeachingPlan, g_ModelTeachers, g_ModelAuditoriums)
		->NaturalJoin(g_ModelLessons)
		->NaturalJoin(g_ModelLessonType)
		->NaturalJoin(g_ModelBranch)
		->NaturalJoin(g_ModelTeachingBranch)
		->NaturalJoin(g_ModelGroupCategory)
		->NaturalJoin(g_ModelSubgroups)
		->NaturalJoin(g_ModelLessonSubgroup)
		->Where(ORM::Eq(g_ModelSchedule.day, id_day) && ORM::Eq(g_ModelSchedule.hour, id_hour) && ORM::Eq(g_ModelGroupCategory.group, id_group) && ORM::Eq(g_ModelSchedule.lesson, g_ModelLessons.fId)
			&& ORM::Eq(g_ModelLessons.teacher, g_ModelTeachers.fId)
			&& ORM::Eq(g_ModelSchedule.auditorium, g_ModelAuditoriums.fId));

	if(data->children().size() == 0)
	{
		return "---";
	}
	else if(data->children().size() == 1)
	{
		return data->children()[0].get_value(lesson_name) + "\n" +
			data->children()[0].get_value(teacher_name) + "\n" +
			data->children()[0].get_value(auditory_name);
	}
	std::cout << "DataBase::GetTimeTableLessonGroupText too many lessons" << std::endl;
	return "!!!";
}

long int DataBase::GetTimeTableLessonGroup(ORM::ForeignKey id_group, ORM::ForeignKey id_hour, ORM::ForeignKey id_day)
{
	ORM::Scheme scheme;
	ORM::Field<ORM::ForeignKey> id(g_ModelLessons);
	scheme.add(id);
	Glib::RefPtr<ORM::Data> data = ORM::Data::create(scheme);

	m_Connection.Select(data
		, g_ModelSchedule.lesson)
	->From(g_ModelSchedule)
	->NaturalJoin(g_ModelLessons)
	->NaturalJoin(g_ModelLessonSubgroup)
	->NaturalJoin(g_ModelSubgroups)
	->NaturalJoin(g_ModelGroupCategory)
	->Where(ORM::Eq(g_ModelSchedule.day, id_day) && ORM::Eq(g_ModelSchedule.hour, id_hour) && ORM::Eq(g_ModelGroupCategory.group, id_group));

	if(data->children().size() == 0)
	{
		return -1;
	}
	else if(data->children().size() == 1)
	{
		return data->children()[0].get_value(id);
	}
	std::cout << "DataBase::GetTimeTableLessonGroup too many lessons" << std::endl;
	return -1;
}

void DataBase::CleanTimeTable()
{
	m_Connection.DeleteFrom(g_ModelSchedule);
}

void DataBase::ListTLHM(Glib::RefPtr<ORM::Data>& data)
{
	m_Connection.SelectDistinct(data
		, g_ModelLessons.teacher
		, g_ModelLessons.fId
		, g_ModelTeachingPlan.hours
		, g_ModelLessonType.multithread)
		->From(g_ModelLessons, g_ModelGroupCategory, g_ModelTeachingPlan)
		->NaturalJoin(g_ModelSubgroups)
		->NaturalJoin(g_ModelTeachingBranch)
		->NaturalJoin(g_ModelLessonType)
		->NaturalJoin(g_ModelLessonSubgroup)
		->Where(ORM::Greater(g_ModelTeachingPlan.hours, 0l))
		->OrderBy(g_ModelLessons.teacher
		, g_ModelLessons.fId);
}

size_t DataBase::CountAuditoriums(bool multithread)
{
	ORM::Scheme scheme;
	ORM::Field<long int> field("");
	scheme.add(field);
	Glib::RefPtr<ORM::Data> data = ORM::Data::create(scheme);
	m_Connection.Select(data
		, ORM::Count())
		->From(g_ModelAuditoriums)
		->Where(ORM::Eq(g_ModelAuditoriums.multithread, multithread));
	if(data->children().size() > 0)
	{
		return data->children()[0].get_value(field);
	}
	return 0;
}

void DataBase::MoveLessons(long int id_lesson_from, long int id_lesson_to)
{
	m_Connection.Update(g_ModelLessonSubgroup)->Set(g_ModelLessonSubgroup.lesson, ORM::ForeignKey(id_lesson_to))->Where(ORM::Eq(g_ModelLessonSubgroup.lesson, ORM::ForeignKey(id_lesson_from)));
	m_Connection.DeleteFrom(g_ModelLessons)->Where(ORM::Eq(g_ModelLessons.fId, ORM::PrimaryKey(id_lesson_from)));
}

bool DataBase::InterseptLessons(long int id_lesson1, long int id_lesson2)
{
	ORM::Subquery groups1;
	Glib::RefPtr<ORM::Data> fake_data;
	groups1.Select(fake_data
		, g_ModelGroupCategory.group)
		->From(g_ModelLessonSubgroup)
		->NaturalJoin(g_ModelSubgroups)
		->NaturalJoin(g_ModelGroupCategory)
		->Where(ORM::Eq(g_ModelLessonSubgroup.lesson, ORM::ForeignKey(id_lesson1)));

	ORM::Subquery groups2;
	groups2.Select(fake_data
		, g_ModelGroupCategory.group)
		->From(g_ModelLessonSubgroup)
		->NaturalJoin(g_ModelSubgroups)
		->NaturalJoin(g_ModelGroupCategory)
		->Where(ORM::Eq(g_ModelLessonSubgroup.lesson, ORM::ForeignKey(id_lesson2)));

	ORM::Scheme scheme;
	ORM::Field<long int> count("");
	scheme.add(count);
	Glib::RefPtr<ORM::Data> data = ORM::Data::create(scheme);
	m_Connection.Select(data
		, ORM::Count())
		->From(g_ModelGroups)
		->Where(ORM::In(g_ModelGroups.fId, groups1)
			&& ORM::In(g_ModelGroups.fId, groups2));
	if(data->children().size() == 1)
	{
		return data->children()[0].get_value(count);
	}
	else
	{
		std::cout << "Wrong records count with COUNT" << std::endl;
	}
	return false;
}

