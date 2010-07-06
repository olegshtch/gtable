#include "lessons_widget.h"
#include "combobox_dialog.h"
#include "db/models.h"

LessonsWidget::LessonsWidget(DB::DataBase &db, const DB::Link_TeachPlan &link)
	:m_DB(db), m_Link(link), m_ParentId(-1), m_Model(ORM::Table::create(DB::g_ModelPlan)), m_ButtonBox(Gtk::BUTTONBOX_SPREAD), m_ListBox(m_Model)
{
	m_ListBox.set_headers_visible(true);
	m_ListBox.append_column("l_id", DB::g_ModelPlan.l_id);
	m_ListBox.append_column("l_name", DB::g_ModelPlan.name);
	m_ListBox.append_column("t_id", DB::g_ModelPlan.t_id);
	m_ListBox.append_column("t_name", DB::g_ModelPlan.t_name);
	m_ListBox.append_column_editable("hours", DB::g_ModelPlan.hours);

	m_Buttons[0].set_label(_("Append"));
	m_ButtonBox.pack_start(m_Buttons[0], Gtk::PACK_SHRINK);
	m_Buttons[0].signal_clicked().connect(sigc::mem_fun(*this, &LessonsWidget::OnAppend));

	m_Buttons[1].set_label(_("Delete"));
	m_ButtonBox.pack_start(m_Buttons[1], Gtk::PACK_SHRINK);
	m_Buttons[1].signal_clicked().connect(sigc::mem_fun(*this, &LessonsWidget::OnDelete));

	m_Scrolled.add(m_ListBox);
	m_Scrolled.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	m_Model->signal_row_changed().connect(sigc::mem_fun(*this, &LessonsWidget::OnEditRow));

	pack_start(m_Scrolled, Gtk::PACK_EXPAND_WIDGET);
	pack_end(m_ButtonBox, Gtk::PACK_SHRINK);

	m_Recurse = false;

	Refresh();
}

void LessonsWidget::Refresh()
{
	m_Recurse = true;
	m_DB.ListLinkedTeachPlan(m_Link, m_ParentId, m_Model);
	show_all_children();
	m_Recurse = false;
}

void LessonsWidget::OnAppend()
{
	Glib::RefPtr<ORM::Table> model = ORM::Table::create(DB::g_ModelLessonTeacher);
	m_DB.ListTeacherLessons(m_Link.m_Entity2, model);
	ComboboxDialog dialog(Glib::RefPtr<Gtk::ListStore>::cast_static(model));
	dialog.PackStart(DB::g_ModelLessonTeacher.name);
	dialog.PackStart(DB::g_ModelLessonTeacher.t_name);
	if(dialog.run() == Gtk::RESPONSE_OK)
	{
		m_DB.AppendLinkedEntity(m_Link, m_ParentId, dialog.GetRow()->get_value(DB::g_ModelLessonTeacher.id));
		Refresh();
	}
}

void LessonsWidget::OnDelete()
{
	Gtk::TreeSelection::ListHandle_Path selected = m_ListBox.get_selection()->get_selected_rows();
	for(Gtk::TreeSelection::ListHandle_Path::const_iterator it = selected.begin(); it != selected.end(); it ++)
	{
		m_DB.DeleteLinkedEntity(m_Link, m_ParentId, m_Model->get_iter(*it)->get_value(DB::g_ModelLessonTeacher.id));
	}
	Refresh();
}

void LessonsWidget::OnEditRow(const Gtk::TreeModel::Path& path, const Gtk::TreeModel::iterator& iter)
{
	if(! m_Recurse)
	{
		m_DB.EditHours(m_Link, iter->get_value(DB::g_ModelPlan.id), iter->get_value(DB::g_ModelPlan.hours));
	}
}

