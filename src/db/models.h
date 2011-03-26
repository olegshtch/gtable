#ifndef _DB_MODELS_H_
#define _DB_MODELS_H_

#include "../orm/table.h"

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

	// id - идентификатор аудитории
	// name - название
	// multithread - многопоточность
	class ModelAud : public ModelEntity
	{
	public:
		ORM::Field<bool> multithread;
			
		ModelAud(const Glib::ustring& table_name)
			:ModelEntity(table_name),
			multithread("multithread")
		{
			add(multithread);
		}
	};

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

	// id - идентификатор первой сущности
	// name - название
	// l_id - идентификатор второй сущности
	// l_name - название
	class ModelOrder : public ModelEntity
	{
	public:
		ORM::Field<long> l_id;
		ORM::Field<Glib::ustring> l_name;

		ModelOrder(const Glib::ustring& table_name)
			:ModelEntity(table_name),
			l_id("l_id"),
			l_name("l_name")
		{
			add(l_id);
			add(l_name);
		}
	};

	// id - идентификатор записи
	// name - название занятия
	// l_id - идентификатор занятия
	// t_id - идентификатор преподавателя
	// t_name - имя преподавателя
	class ModelLessonTeacher : public ModelEntity
	{
	public:
		ORM::Field<long> l_id;
		ORM::Field<long> t_id;
		ORM::Field<Glib::ustring> t_name;

		ModelLessonTeacher(const Glib::ustring& table_name)
			:ModelEntity(table_name),
			l_id("l_id"),
			t_id("t_id"),
			t_name("t_name")
		{
			add(l_id);
			add(t_id);
			add(t_name);
		}
	};

	// id - идентификатор записи
	// name - название занятия
	// l_id - идентификатор занятия
	// t_id - идентификатор преподавателя
	// t_name - имя преподавателя
	// hours - количество часов	
	class ModelPlan : public ModelLessonTeacher
	{
	public:
		ORM::Field<long> hours;

		ModelPlan(const Glib::ustring& table_name)
			:ModelLessonTeacher(table_name),
			hours("hours")
		{
			add(hours);
		}
	};

	// id - идентификатор записи в TeachPlan
	// t_id - идентификатор преподавателя
	// t_name - имя преподавателя
	// l_id - идентификатор занятия
	// l_name - название занятия
	// g_id - идентификатор группы
	// g_name - название группы
	// hours - количество часов
	class ModelLessonRecords : public ORM::Scheme
	{
	public:
		ORM::Field<long> id;
		ORM::Field<long> t_id;
		ORM::Field<Glib::ustring> t_name;
		ORM::Field<long> l_id;
		ORM::Field<Glib::ustring> l_name;
		ORM::Field<long> g_id;
		ORM::Field<Glib::ustring> g_name;
		ORM::Field<long> hours;
		
		ModelLessonRecords()
			:id("id"),
			t_id("t_id"),
			t_name("t_name"),
			l_id("l_id"),
			l_name("l_name"),
			g_id("g_id"),
			g_name("g_name"),
			hours("hours")
		{
			add(id);
			add(t_id);
			add(t_name);
			add(l_id);
			add(l_name);
			add(g_id);
			add(g_name);
			add(hours);
		}
	};

	extern const ModelAud g_ModelAud;
	extern const ModelEntity g_ModelDays;
	extern const ModelEntity g_ModelHours;
	extern const ModelEntity g_ModelGroups;
	extern const ModelEntity g_ModelTeachers;
	extern const ModelEntity g_ModelLessons;
	extern const ModelFaculties g_ModelFaculties;

	extern const ModelOrder g_ModelOrder;
	extern const ModelPlan g_ModelPlan;
	extern const ModelLessonTeacher g_ModelLessonTeacher;
	extern const ModelLessonRecords g_ModelLessonRecords;
	
}

#endif

