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
		ORM::Field<ORM::ForeignKey> fTeacher;

		LessonColumnRecord()
			:fId(),
			fLesson(""),
			fTeacher(DB::g_ModelTeachers)
		{
			add(fId);
			add(fLesson);
			add(fTeacher);
		}
	};

	void OnTeacherEdited(const Glib::ustring& path, long int id);
	virtual bool on_button_release_event(GdkEventButton *event);
	void OnAddToStreamActivate(long int id_stream_to, long int id_stream_from);
	
	LessonColumnRecord m_LessonColumnRecord;
	Glib::RefPtr<ORM::Data> m_Model;

	Gtk::TreeViewColumn m_TeacherColumn;
	CellRendererForeign m_TeacherRenderer;
	Gtk::Menu m_Menu;
	long int m_IdSubgroup;
};

#endif

