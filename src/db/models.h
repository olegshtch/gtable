#ifndef _DB_MODELS_H_
#define _DB_MODELS_H_

//#include <gtkmm/treemodel.h>
#include "../orm/scheme.h"

namespace DB
{

	// id - идентификатор сущности
	// name - название
	class ModelEntity : public ORM::Scheme
	{
	public:
		ORM::Field<long> id;
		ORM::Field<Glib::ustring> name;

		ModelEntity()
		{
			add(id);
			add(name);
		}

//		static int Callback(void *list_ptr, int argc, char **argv, char **col_name);

	};

	// id - идентификатор аудитории
	// name - название
	// multithread - многопоточность
	class ModelAud : public ModelEntity
	{
	public:
		ORM::Field<bool> multithread;
			
		ModelAud()
		{
			add(multithread);
		}

//		static int Callback(void *list_ptr, int argc, char **argv, char **col_name);

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

		ModelOrder()
		{
			add(l_id);
			add(l_name);
		}

//		static int Callback(void *list_ptr, int argc, char **argv, char **col_name);

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

		ModelLessonTeacher()
		{
			add(l_id);
			add(t_id);
			add(t_name);
		}

//		static int Callback(void *list_ptr, int argc, char **argv, char **col_name);


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

		ModelPlan()
		{
			add(hours);
		}

//		static int Callback(void *list_ptr, int argc, char **argv, char **col_name);

	};

	extern const ModelEntity g_ModelEntity;
	extern const ModelAud g_ModelAud;
	extern const ModelOrder g_ModelOrder;
	extern const ModelPlan g_ModelPlan;
	extern const ModelLessonTeacher g_ModelLessonTeacher;
	
}

#endif

