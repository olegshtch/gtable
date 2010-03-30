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

namespace DB
{
	class DataBase
	{
	public:
		DataBase(const Glib::ustring &db, bool create_new);
		~DataBase();
		void AppendEntity(const Entity& ent, const Glib::ustring &name);
		void ListEntity(const Entity& ent, Glib::RefPtr<Gtk::ListStore> &list_store);
		void DeleteEntity(const Entity& ent, int id);
		void EditEntityName(const Entity& ent, int id, const Glib::ustring &new_name);

		void EditMultithr(const Entity& ent, int id, bool multithr);
		void ListEntityAud(const Entity& ent, Glib::RefPtr<Gtk::ListStore> &list_store);

		void ListLinkedEntity(const Link_N2N& link, int parent_id, Glib::RefPtr<Gtk::ListStore> &list_store);
		void AppendLinkedEntity(const Link_N2N& link, int parent_id, int child_id);
		void DeleteLinkedEntity(const Link_N2N& link, int parent_id, int child_id);
		void ListLink(const Link_N2N& link, Glib::RefPtr<Gtk::ListStore> &list_store);
		void ListLinkedTeachPlan(const Link_TeachPlan& link, int parent_id, Glib::RefPtr<Gtk::ListStore> &list_store);
		void ListTeacherLessons(const DB::Link_N2N& link, Glib::RefPtr<Gtk::ListStore>& list_store);

		void EditHours(const DB::Link_TeachPlan& link, int id, unsigned int hours);
		//void ListCircleLink(const Link_N2N& link, Glib::RefPtr<Gtk::ListStore> &list_store);
		
		size_t GetEntitiesCount(const DB::Entity& ent);
		size_t GetEntitiesIDs(const DB::Entity& ent, std::vector<size_t> *array);
		bool IsLinkBetween(const DB::Link_N2N &link, size_t id_a, size_t id_l);
		size_t GetTForGL(size_t id_g, size_t id_l); // get teacher id by lesson's id and group's id
	private:
		void SQLExec0(const Glib::ustring& sql);
		static int SQLCallBack0(void *self_ptr, int argc, char **argv, char **col_name);
		template <class Model> void SQLExec(const Glib::ustring& sql, Glib::RefPtr<Gtk::ListStore> &list_store)
		{
			char *err_msg = 0;
			std::clog << sql << std::endl;
			list_store->clear();
			int sql_answer = sqlite3_exec(m_SQLite, sql.c_str(), Model::Callback, DeRef(list_store), &err_msg);
			if(sql_answer != SQLITE_OK)
			{
				Glib::ustring msg(err_msg);
				sqlite3_free(err_msg);
				throw Glib::Error(1, 0, msg);
			}
		}

		template <class Type> void SQLExec(const Glib::ustring& sql, Type *result)
		{
			char *err_msg = 0;
			std::clog << sql << std::endl;
			int sql_answer = sqlite3_exec(m_SQLite, sql.c_str(), Callback<Type>, result, &err_msg);
			if(sql_answer != SQLITE_OK)
			{
				Glib::ustring msg(err_msg);
				sqlite3_free(err_msg);
				throw Glib::Error(1, 0, msg);
			}
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

		template <class Type> size_t SQLExecArray(const Glib::ustring& sql, std::vector<Type> *array)
		{
			std::pair<size_t, std::vector<Type>*> res;
			res.first = 0;
			res.second = array;
			char *err_msg = 0;
			std::clog << sql << std::endl;
			int sql_answer = sqlite3_exec(m_SQLite, sql.c_str(), CallbackArray<Type>, &res, &err_msg);
			if(sql_answer != SQLITE_OK)
			{
				Glib::ustring msg(err_msg);
				sqlite3_free(err_msg);
				throw Glib::Error(1, 0, msg);
			}
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

		void CreateTableEntity(const Entity& ent);
		void CreateTableLinkN2N(const Link_N2N& link);
		void CreateTableLinkTeachPlan(const Link_TeachPlan& link);
		void CreateTableSchedule();
		
		sqlite3 *m_SQLite;
	};
}

#endif

