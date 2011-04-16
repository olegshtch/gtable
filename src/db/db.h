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
			ORM::Table::InitTables(m_Connection);
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
		void ListEntitiesText(const ORM::Table& ent, const ORM::Field<Glib::ustring> field, Glib::RefPtr<ORM::Data> &data);
		void EditEntity(const ORM::Table& ent, const Gtk::TreeIter& row);
		void RemoveEntity(const ORM::Table& ent, const Gtk::TreeIter& row);
		void EditEntityName(const ModelEntity& ent, int id, const Glib::ustring &new_name);
		Glib::ustring GetEntityName(const ModelEntity& ent, int id);

		void EditMultithr(const ModelEntity& ent, int id, bool multithr);
		bool GetAudMultithr(const ModelEntity& ent, long id);

		size_t GetEntitiesCount(const DB::ModelEntity& ent);
		size_t GetEntitiesIDs(const DB::ModelEntity& ent, std::vector<size_t> *array);

		Glib::ustring GetTextById(const ORM::Table& ent, const ORM::Field<Glib::ustring>& field, long id);

	private:		
		ORM::Connection m_Connection;
	};
}

#endif

