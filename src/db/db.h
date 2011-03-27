#ifndef _DB_H_
#define _DB_H_

#include <glibmm/ustring.h>
#include <gtkmm/liststore.h>
#include <sqlite3.h>
#include <iostream>
#include <sstream>
#include "../shared.h"
#include "../orm/connection.h"
#include "../orm/table.h"
#include "models.h"

namespace DB
{
	class DataBase
	{
	private:
		DataBase(const Glib::ustring &db);
		~DataBase()
		{
		}
		DataBase(const DataBase&);
		DataBase& operator=(const DataBase&);
		static DataBase* s_Ptr;
		static void Free();
	public:
		static DataBase& Instance()
		{
			return *s_Ptr;
		}

		void New()
		{
			m_Connection.~Connection();
			new (&m_Connection) ORM::Connection(":memory:", true);	
			InitTables();
		}
		void Open(const Glib::ustring &dbname)
		{
			m_Connection.~Connection();
			new (&m_Connection) ORM::Connection(dbname, false);	
		}
		void Save(const Glib::ustring &dbname)
		{
			m_Connection.MoveTo(dbname);
		}

		void AppendEntity(const ORM::Table& ent, const Gtk::TreeIter& row);
		void ListEntity(const ORM::Table& ent, Glib::RefPtr<ORM::Data> &list_store);
		void EditEntity(const ORM::Table& ent, const Gtk::TreeIter& row);
		void DeleteEntity(const ModelEntity& ent, int id);
		void EditEntityName(const ModelEntity& ent, int id, const Glib::ustring &new_name);
		Glib::ustring GetEntityName(const ModelEntity& ent, int id);

		void EditMultithr(const ModelEntity& ent, int id, bool multithr);
		bool GetAudMultithr(const ModelEntity& ent, long id);

		size_t GetEntitiesCount(const DB::ModelEntity& ent);
		size_t GetEntitiesIDs(const DB::ModelEntity& ent, std::vector<size_t> *array);

		Glib::ustring GetTextById(const ORM::Table& ent, const ORM::Field<Glib::ustring>& field, long id);

		//bool IsLinkBetween(const DB::Link_N2N &link, size_t id_a, size_t id_l);
		//size_t GetTForGL(size_t id_g, size_t id_l); // get teacher id by lesson's id and group's id

		void GetGTList(Glib::RefPtr<ORM::Data> &list_store);
		//void GetALList(Glib::RefPtr<ORM::Data> &list_store);

		void ListLessonRecords(Glib::RefPtr<ORM::Data> &list_store);
	private:
		void InitTables();
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

		//void CreateTableEntity(const ModelEntity& ent);
		//void CreateTableLinkN2N(const Link_N2N& link);
		//void CreateTableLinkTeachPlan(const Link_TeachPlan& link);
		//void CreateTableSchedule();
		
		ORM::Connection m_Connection;
	};
}

#endif

