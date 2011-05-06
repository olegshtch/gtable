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

		void New();
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
		void ListEntitiesText(const ORM::Table& ent, const ORM::Expr<Glib::ustring> field, Glib::RefPtr<ORM::Data> &data);
		void ListIDs(const ORM::Table& ent, Glib::RefPtr<ORM::Data> &data);
		void EditEntity(const ORM::Table& ent, const Gtk::TreeIter& row);
		void RemoveEntity(const ORM::Table& ent, const Gtk::TreeIter& row);
		Glib::ustring GetTextById(const ORM::Table& ent, const ORM::Expr<Glib::ustring>& field, long id);

		bool GetTeacherHolydays(long int teacher_id, long int day_id, long int hour_id);
		void SetTeacherHolydays(long int teacher_id, long int day_id, long int hour_id, bool holyday);

		bool GetGroupHolydays(long int group_id, long int day_id, long int hour_id);
		void SetGroupHolydays(long int group_id, long int day_id, long int hour_id, bool holyday);

		bool GetWeeks();
		void SetWeeks(bool weeks);

		void GetTeachingBranch(Glib::RefPtr<ORM::Data> &data, long int id_speciality);
		void ListNewBranchForSpeciality(Glib::RefPtr<ORM::Data> &data, long int id_speciality);
		void AppendNewBranchForSpeciality(long int id_speciality, long int id_branch);

		long GetTeachingPlanHours(long int id_teaching_branch, long int id_lesson_type);
		void EditTeachingPlanHours(long int id_teaching_branch, long int id_lesson_type, long hours);

		void GetSubgroupsList(Glib::RefPtr<ORM::Data>& data);
		void GetLessonsForSubgroup(Glib::RefPtr<ORM::Data>& data, const ORM::ForeignKey& id_subgroup);
		void SetLessonTeacher(ORM::PrimaryKey id_lesson, ORM::ForeignKey id_teacher);
		void GetStreamsListForAdding(Glib::RefPtr<ORM::Data>& data, const ORM::ForeignKey& id_subgroup, const ORM::PrimaryKey& id_stream);

		void ListGroupOtherLessons(long int id_group, Glib::RefPtr<ORM::Data>& data);
		void GetAuditoriumListForLesson(Glib::RefPtr<ORM::Data>& data, ORM::PrimaryKey lesson_id, ORM::ForeignKey day_id, ORM::ForeignKey hour_id);
		void SetLessonIntoTimetable(long int id_lesson, long int id_aud, long int id_hour, long int id_day);
		void SetStreamIntoTimetable(long int id_stream, long int id_aud, long int id_hour, long int id_day);
		Glib::ustring GetTimeTableLessonGroup(ORM::ForeignKey id_group, ORM::ForeignKey id_hour, ORM::ForeignKey id_day);
		void CleanTimeTable();

		void ListTSHM(Glib::RefPtr<ORM::Data>& data);
		size_t CountAuditoriums(bool multithread);

		void MoveStreams(long int id_stream_from, long int id_stream_to);
		bool InterseptStreams(long int id_stream1, long int id_stream2);
	private:		
		ORM::Connection m_Connection;
	};
}

#endif

