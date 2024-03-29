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
		long int AppendEntityByName(const DB::ModelFaculties& ent, const Glib::ustring& name);
		void ListEntityOrdered(const ORM::Table& ent, Glib::RefPtr<ORM::Data> &list_store, const ORM::ExprBase& sort_expr);
		void ListEntitiesTextOrdered(const ORM::Table& ent, const ORM::Expr<Glib::ustring> field, Glib::RefPtr<ORM::Data> &data);
		void ListEntitiesTextOrderedID(const ORM::Table& ent, const ORM::Expr<Glib::ustring> field, Glib::RefPtr<ORM::Data> &data);
		void ListIDs(const ORM::Table& ent, Glib::RefPtr<ORM::Data> &data);
		template<class T> void EditEntityField(const ORM::Table& ent, ORM::PrimaryKey id, const ORM::Field<T>& field, const T& value)
		{
			m_Connection.Update(ent)->Set(field, value)->Where(ORM::Eq(ent.fId, id));
		}
		void RemoveEntity(const ORM::Table& ent, const Gtk::TreeIter& row);
		Glib::ustring GetTextById(const ORM::Table& ent, const ORM::Expr<Glib::ustring>& field, long id);

		bool GetTeacherHolydays(long int teacher_id, long int day_id, long int hour_id);
		void SetTeacherHolydays(long int teacher_id, long int day_id, long int hour_id, bool holyday);

		bool GetGroupHolydays(long int group_id, long int day_id, long int hour_id);
		void SetGroupHolydays(long int group_id, long int day_id, long int hour_id, bool holyday);

		bool GetAuditoriumHolydays(long int auditorium_id, long int day_id, long int hour_id);
		void SetAuditoriumHolydays(long int auditorium_id, long int day_id, long int hour_id, bool holyday);

		bool GetWeeks();
		void SetWeeks(bool weeks);

		void GetTeachingBranch(Glib::RefPtr<ORM::Data> &data, long int id_speciality);
		void ListNewBranchForSpeciality(Glib::RefPtr<ORM::Data> &data, long int id_speciality);

		/// \return TeachingBranch;
		long int AppendNewBranchForSpeciality(long int id_speciality, long int id_branch);

		void RemoveBranchForSpeciality(ORM::ForeignKey id_speciality, ORM::PrimaryKey id_branch);

		long GetTeachingPlanHours(long int id_teaching_branch, long int id_lesson_type);
		void EditTeachingPlanHours(long int id_teaching_branch, long int id_lesson_type, long hours);

		void GetSubgroupsList(Glib::RefPtr<ORM::Data>& data);
		void GetLessonsForSubgroup(Glib::RefPtr<ORM::Data>& data, const ORM::ForeignKey& id_subgroup);
		void SetLessonTeacher(ORM::PrimaryKey id_lesson, ORM::ForeignKey id_teacher);
		void GetLessonsListForAdding(Glib::RefPtr<ORM::Data>& data, const ORM::ForeignKey& id_subgroup, const ORM::PrimaryKey& id_lesson);

		void ListGroupOtherLessons(long int id_group, Glib::RefPtr<ORM::Data>& data);
		void GetAuditoriumListForLesson(Glib::RefPtr<ORM::Data>& data, ORM::PrimaryKey lesson_id, ORM::ForeignKey day_id, ORM::ForeignKey hour_id);
		bool SetLessonIntoTimetable(long int id_lesson, ORM::ForeignKey id_aud, ORM::ForeignKey day_id, ORM::ForeignKey hour_id);
		bool CheckLessonIntoTimetable(long int id_lesson, ORM::ForeignKey day_id, ORM::ForeignKey hour_id);
		void RemoveLessonFromTimetable(long int id_lesson, long int id_day, long int id_hour);
		Glib::ustring GetTimeTableLessonGroupText(ORM::ForeignKey id_group, ORM::ForeignKey id_hour, ORM::ForeignKey id_day, bool show_auditorium = true);
		long int GetTimeTableLessonGroup(ORM::ForeignKey id_group, ORM::ForeignKey id_hour, ORM::ForeignKey id_day);
		void CleanTimeTable();

		void ListTeacherOtherLessons(long int id_teacher, Glib::RefPtr<ORM::Data>& data);
		Glib::ustring GetTimeTableLessonTeacherText(ORM::ForeignKey id_teacher, ORM::ForeignKey id_hour, ORM::ForeignKey id_day);
		long int GetTimeTableLessonTeacher(ORM::ForeignKey id_teacher, ORM::ForeignKey id_hour, ORM::ForeignKey id_day);

		void ListAuditoriumOtherLessons(long int id_aud, Glib::RefPtr<ORM::Data>& data);
		Glib::ustring GetTimeTableLessonAuditoriumText(ORM::ForeignKey id_aud, ORM::ForeignKey id_hour, ORM::ForeignKey id_day);
		long int GetTimeTableLessonAuditorium(ORM::ForeignKey id_aud, ORM::ForeignKey id_hour, ORM::ForeignKey id_day);

		void ListTLBHM(Glib::RefPtr<ORM::Data>& data);
		size_t CountAuditoriums(bool multithread, ORM::ForeignKey id_day, ORM::ForeignKey id_hour);

		bool GetLessonHolydays(long int lesson_id, long int day_id, long int hour_id);

		void MoveLessons(long int id_lesson_from, long int id_lesson_to);
		bool InterseptGroups(long int id_lesson1, long int id_lesson2);
		Glib::ustring GetStreamByLesson(ORM::ForeignKey id_lesson);
		bool IsGroupsInStream(long int id_lesson, long int id_subgroup);
		void RemoveSubgroupFromLesson(long int id_lesson, long int id_subgroup);
	private:		
		ORM::Connection m_Connection;
	};
}

#endif

