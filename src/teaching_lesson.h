#ifndef _TEACHING_LESSON_H_
#define _TEACHING_LESSON_H_

#include <gtkmm/treeview.h>
#include <gtkmm/builder.h>
#include <gtkmm/menu.h>
#include "cellrendererforeign.h"
#include "orm/scheme.h"
#include "orm/expr.h"
#include "db/db.h"

class TeachingLesson : public Gtk::TreeView
{
public:
	TeachingLesson(GtkTreeView *cobject, const Glib::RefPtr<Gtk::Builder>& builder);
	~TeachingLesson()
	{
	}

	void SetSubgroup(long int id_subgroup);
private:
	class LessonColumnRecord : public ORM::Scheme
	{
	public:
		ORM::Field<ORM::PrimaryKey> fId;
		ORM::Field<Glib::ustring> fLesson;
		//ORM::Field<Glib::ustring> fStream;
		ORM::Field<ORM::ForeignKey> fTeacher;

		LessonColumnRecord()
			:fId(),
			fLesson(""),
			//fStream(""),
			fTeacher(DB::g_ModelTeachers)
		{
			add(fId);
			add(fLesson);
			//add(fStream);
			add(fTeacher);
		}
	};

	void OnTeacherEdited(const Glib::ustring& path, long int id);
	virtual bool on_button_release_event(GdkEventButton *event);
	void OnAddToStreamActivate(long int id_stream_to, long int id_stream_from);
	void OnDelFromStreamActivate(long int id_stream);
	static void LessonAsStream(Gtk::CellRenderer *cell, const Gtk::TreeModel::iterator& row);
	
	static LessonColumnRecord s_LessonColumnRecord;
	Glib::RefPtr<ORM::Data> m_Model;

	Gtk::TreeViewColumn m_TeacherColumn;
	CellRendererForeign m_TeacherRenderer;
	Gtk::TreeViewColumn m_StreamColumn;
	Gtk::CellRendererText m_StreamRenderer;
	Gtk::Menu m_Menu;
	long int m_IdSubgroup;
};

#endif

