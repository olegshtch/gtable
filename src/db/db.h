#ifndef _DB_H_
#define _DB_H_

#include <glibmm/ustring.h>
#include <gtkmm/liststore.h>
#include <sqlite3.h>
#include <iostream>
#include <sstream>
#include "entities.h"
#include "links.h"
#include "../shared.h"
#include "../orm/connection.h"

namespace DB
{
	class DataBase
	{
	public:
		DataBase(const Glib::ustring &db, bool create_new);
		~DataBase()
		{
		}
		void AppendEntity(const Entity& ent, const Glib::ustring &name);
		void ListEntity(const Entity& ent, Glib::RefPtr<ORM::Table> &list_store);
		void DeleteEntity(const Entity& ent, int id);
		void EditEntityName(const Entity& ent, int id, const Glib::ustring &new_name);
		Glib::ustring GetEntityName(const Entity& ent, int id);

		void EditMultithr(const Entity& ent, int id, bool multithr);
		void ListEntityAud(const Entity& ent, Glib::RefPtr<ORM::Table> &list_store);
		bool GetAudMultithr(const Entity& ent, long id);

		void ListLinkedEntity(const Link_N2N& link, int parent_id, Glib::RefPtr<ORM::Table> &list_store);
		void AppendLinkedEntity(const Link_N2N& link, int parent_id, int child_id);
		void DeleteLinkedEntity(const Link_N2N& link, int parent_id, int child_id);
		void ListLink(const Link_N2N& link, Glib::RefPtr<ORM::Table> &list_store);
		void ListLinkedTeachPlan(const Link_TeachPlan& link, int parent_id, Glib::RefPtr<ORM::Table> &list_store);
		void ListTeacherLessons(const DB::Link_N2N& link, Glib::RefPtr<ORM::Table>& list_store);

		void EditHours(const DB::Link_TeachPlan& link, int id, unsigned int hours);
		//void ListCircleLink(const Link_N2N& link, Glib::RefPtr<Gtk::ListStore> &list_store);
		
		size_t GetEntitiesCount(const DB::Entity& ent);
		size_t GetEntitiesIDs(const DB::Entity& ent, std::vector<size_t> *array);
		bool IsLinkBetween(const DB::Link_N2N &link, size_t id_a, size_t id_l);
		size_t GetTForGL(size_t id_g, size_t id_l); // get teacher id by lesson's id and group's id

		void GetGTList(Glib::RefPtr<ORM::Table> &list_store);
		void GetALList(Glib::RefPtr<ORM::Table> &list_store);

		void ListLessonRecords(Glib::RefPtr<ORM::Table> &list_store);
	private:
#if 0
		void SQLExec0(const Glib::ustring& sql);
		static int SQLCallBack0(void *self_ptr, int argc, char **argv, char **col_name);

		template <class Model> void SQLExec(const Glib::ustring& sql, Glib::RefPtr<Gtk::ListStore> &list_store)
		{
			list_store->clear();
			int sql_answer = sqlite3_exec(m_SQLite, sql.c_str(), Model::Callback, DeRef(list_store), &err_msg);
		}
#endif

#if 1
		template <class Type> void SQLExec(const Glib::ustring& sql, Type *result)
		{
			m_Connection.SQLExecOwn(sql, Callback<Type>, reinterpret_cast<void*>(result));
		}

		template <class Type> static int Callback(void *result, int argc, char **argv, char **col_name)
		{
			if(! result)
			{
				throw Glib::Error(1, 0, "Null poiter to result at DB::Callback<Type>.");
			}
			if(argc != 1)
			{
				throw Glib::Error(1, 0, "Not 1 columns at DB::Callback<Type>.");
			}
			Type &res = *reinterpret_cast<Type*>(result);
			std::clog << " [" << col_name[0] << "]=" << argv[0] << std::endl;
			std::stringstream stream(argv[0]);
			stream >> res;
			return 0;
		}

		void SQLExecString(const Glib::ustring& sql, Glib::ustring *result)
		{
			m_Connection.SQLExecOwn(sql, CallbackString, reinterpret_cast<void*>(result));
		}

		static int CallbackString(void *result, int argc, char **argv, char **col_name)
		{
			if(! result)
			{
				throw Glib::Error(1, 0, "Null poiter to result at DB::Callback<Type>.");
			}
			if(argc != 1)
			{
				throw Glib::Error(1, 0, "Not 1 columns at DB::Callback<Type>.");
			}
			Glib::ustring &res = *reinterpret_cast<Glib::ustring*>(result);
			std::clog << " [" << col_name[0] << "]=" << argv[0] << std::endl;
			res = argv[0];
			return 0;
		}


		template <class Type> size_t SQLExecArray(const Glib::ustring& sql, std::vector<Type> *array)
		{
			std::pair<size_t, std::vector<Type>*> res;
			res.first = 0;
			res.second = array;
			m_Connection.SQLExecOwn(sql, CallbackArray<Type>, reinterpret_cast<void*>(&res));
			return res.first;
		}

		template <class Type> static int CallbackArray(void *result, int argc, char **argv, char **col_name)
		{
			if(! result)
			{
				throw Glib::Error(1, 0, "Null poiter to result at DB::CallbackArray<Type>.");
			}
			if(argc != 1)
			{
				throw Glib::Error(1, 0, "Not 1 columns at DB::CallbackArray<Type>.");
			}
			std::pair<size_t, std::vector<Type> *> *res = reinterpret_cast<std::pair<size_t, std::vector<Type>*> *>(result);
			std::clog << " [" << col_name[0] << "]=" << argv[0] << std::endl;
			std::stringstream stream(argv[0]);
			stream >> res->second->at(res->first);
			res->first ++;
			return 0;
		}
#endif

		void CreateTableEntity(const Entity& ent);
		void CreateTableLinkN2N(const Link_N2N& link);
		void CreateTableLinkTeachPlan(const Link_TeachPlan& link);
		void CreateTableSchedule();
		
		ORM::Connection m_Connection;
	};
}

#endif

