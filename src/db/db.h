#ifndef _DB_H_
#define _DB_H_

#include <glibmm/ustring.h>
#include <gtkmm/liststore.h>
#include <sqlite3.h>
#include <iostream>
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

		void CreateTableEntity(const Entity& ent);
		void CreateTableLinkN2N(const Link_N2N& link);
		void CreateTableLinkTeachPlan(const Link_TeachPlan& link);
		void CreateTableSchedule();
		
		sqlite3 *m_SQLite;
	};
}

#endif

