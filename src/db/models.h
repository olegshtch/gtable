#ifndef _DB_MODELS_H_
#define _DB_MODELS_H_

#include "../orm/table.h"
#include "../orm/foreign_key.h"

namespace DB
{

	// id - идентификатор сущности
	// name - название
	class ModelEntity : public ORM::Table
	{
	public:
		ORM::Field<Glib::ustring> name;

		ModelEntity(const Glib::ustring& table_name)
			:ORM::Table(table_name),
			name("name")
		{
			add(name);
		}
	};

	extern const ModelEntity g_ModelDays;
	extern const ModelEntity g_ModelBranchCategory;

	class ModelBranch : public ModelEntity
	{
	public:
		ORM::Field<ORM::ForeignKey> category;

		ModelBranch(const Glib::ustring& table_name)
			:ModelEntity(table_name),
			category(g_ModelBranchCategory)
		{
			add(category);
		}
	};

	extern const ModelBranch g_ModelBranch;

	// id - идентификатор сущности
	// start - время начала
	// finish - время окончания
	class ModelHours : public ORM::Table
	{
	public:
		ORM::Field<Glib::ustring> start;
		ORM::Field<Glib::ustring> finish;

		ModelHours(const Glib::ustring& table_name)
			:ORM::Table(table_name),
			start("start"),
			finish("finish")
		{
			add(start);
			add(finish);
		}
	};

	extern const ModelHours g_ModelHours;

	extern const ModelEntity g_ModelBuildings;
	
	// id - идентификатор сущности
	// name - название
	// abbr - сокращение
	class ModelFaculties : public ModelEntity
	{
	public:
		ORM::Field<Glib::ustring> abbr;

		ModelFaculties(const Glib::ustring& table_name)
			:ModelEntity(table_name),
			abbr("abbr")
		{
			add(abbr);
		}
	};

	extern const ModelFaculties g_ModelFaculties;

	// id - идентификатор кафедры
	// name - название
	// abbr - сокращение
	// faculty - факультет
	class ModelChairs : public ModelFaculties
	{
	public:
		ORM::Field<ORM::ForeignKey> faculty;

		ModelChairs(const Glib::ustring& table_name)
			:ModelFaculties(table_name),
			faculty(g_ModelFaculties)
		{
			add(faculty);
		}
	};

	extern const ModelChairs g_ModelChairs;

	// id - идентификатор аудитории
	// name - название
	// capacity - вместимость
	// multithread - многопоточность
	// building - корпус
	// chair - кафедра
	class ModelAuditoriums : public ModelEntity
	{
	public:
		ORM::Field<long> capacity;
		ORM::Field<bool> multithread;
		ORM::Field<ORM::ForeignKey> building;
		ORM::Field<ORM::ForeignKey> chair;
			
		ModelAuditoriums(const Glib::ustring& table_name)
			:ModelEntity(table_name),
			capacity("capacity"),
			multithread("multithread"),
			building(g_ModelBuildings),
			chair(g_ModelChairs)
		{
			add(capacity);
			add(multithread);
			add(building);
			add(chair);
		}
	};

	extern const ModelAuditoriums g_ModelAuditoriums;

	// id - идентификатор специальности
	// name - название
	// abbr - сокращение
	// chair - кафедра
	class ModelSpecialities : public ModelFaculties
	{
	public:
		ORM::Field<ORM::ForeignKey> chair;
		ModelSpecialities(const Glib::ustring& table_name)
			:ModelFaculties(table_name),
			chair(g_ModelChairs)
		{
			add(chair);
		}
	};

	extern const ModelSpecialities g_ModelSpecialities;

	// id - идентификатор группы
	// name - имя группы
	// speciality - специальность
	class ModelGroups : public ModelEntity
	{
	public:
		ORM::Field<ORM::ForeignKey> speciality;
			
		ModelGroups(const Glib::ustring& table_name)
			:ModelEntity(table_name),
			speciality(g_ModelSpecialities)
		{
			add(speciality);
		}
	};

	extern const ModelGroups g_ModelGroups;

	// group - идентификаторв преподавателя
	// day - идентификатор дня
	// hour - идентификатор часа
	class ModelGroupHolydays : public ORM::Table
	{
	public:
		ORM::Field<ORM::ForeignKey> group;
		ORM::Field<ORM::ForeignKey> day;
		ORM::Field<ORM::ForeignKey> hour;

		ModelGroupHolydays(const Glib::ustring& table_name)
			:ORM::Table(table_name),
			group(DB::g_ModelGroups),
			day(DB::g_ModelDays),
			hour(DB::g_ModelHours)
		{
			add(group);
			add(day);
			add(hour);
		}
	};

	extern const ModelGroupHolydays g_ModelGroupHolydays;

	// id - идентификатор преподавателя
	// firstname - имя
	// secondname - фамилия
	// thirdname - отчество
	class ModelTeachers : public ORM::Table
	{
	public:
		ORM::Field<Glib::ustring> firstname;
		ORM::Field<Glib::ustring> secondname;
		ORM::Field<Glib::ustring> thirdname;
		ORM::Field<ORM::ForeignKey> chair;

		ModelTeachers(const Glib::ustring& table_name)
			:ORM::Table(table_name),
			firstname("firstname"),
			secondname("secondname"),
			thirdname("thirdname"),
			chair(g_ModelChairs)
		{
			add(firstname);
			add(secondname);
			add(thirdname);
			add(chair);
		}
	};

	extern const ModelTeachers g_ModelTeachers;

	// teacher - идентификаторв преподавателя
	// day - идентификатор дня
	// hour - идентификатор часа
	class ModelTeacherHolydays : public ORM::Table
	{
	public:
		ORM::Field<ORM::ForeignKey> teacher;
		ORM::Field<ORM::ForeignKey> day;
		ORM::Field<ORM::ForeignKey> hour;

		ModelTeacherHolydays(const Glib::ustring& table_name)
			:ORM::Table(table_name),
			teacher(DB::g_ModelTeachers),
			day(DB::g_ModelDays),
			hour(DB::g_ModelHours)
		{
			add(teacher);
			add(day);
			add(hour);
		}
	};

	extern const ModelTeacherHolydays g_ModelTeacherHolydays;

	class ModelWeek : public ORM::Table
	{
	public:
		ORM::Field<bool> doubleweek;

		ModelWeek(const Glib::ustring& table_name)
			:ORM::Table(table_name),
			doubleweek("doubleweek")
		{
			add(doubleweek);
		}
	};

	extern const ModelWeek g_ModelWeek;

	typedef ModelFaculties ModelLessonType;

	extern const ModelLessonType g_ModelLessonType;

	class ModelTeachingBranch : public ORM::Table
	{
	public:
		ORM::Field<ORM::ForeignKey> speciality;
		ORM::Field<ORM::ForeignKey> branch;

		ModelTeachingBranch(const Glib::ustring& table_name)
			:ORM::Table(table_name),
			speciality(g_ModelSpecialities),
			branch(g_ModelBranch)
		{
			add(speciality);
			add(branch);
		}
	};

	extern const ModelTeachingBranch g_ModelTeachingBranch;

	class ModelTeachingPlan : public ORM::Table
	{
	public:
		ORM::Field<ORM::ForeignKey> teaching_branch;
		ORM::Field<ORM::ForeignKey> lesson_type;
		ORM::Field<long int> hours;

		ModelTeachingPlan(const Glib::ustring& table_name)
			:ORM::Table(table_name),
			teaching_branch(g_ModelTeachingBranch),
			lesson_type(g_ModelLessonType),
			hours("hours")
		{
			add(teaching_branch);
			add(lesson_type);
			add(hours);
		}
	};

	extern const ModelTeachingPlan g_ModelTeachingPlan;
}

#endif

