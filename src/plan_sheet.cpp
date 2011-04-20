#include <gtkmm/dialog.h>
#include <gtkmm/stock.h>
#include "plan_sheet.h"
#include "db/db.h"

PlanSheet::PlanSheet(GtkTreeView *cobject, const Glib::RefPtr<Gtk::Builder> &builder)
	:Glib::ObjectBase(typeid(PlanSheet)),
	Sheet(cobject, builder),
	m_BranchRenderer(DB::g_ModelTeachingBranch.branch, DB::g_ModelBranch, DB::g_ModelBranch.name),
	m_IdSpeciality(-1)
{
	remove_all_columns();
	insert_column_with_data_func(0, Glib::ustring(), m_BranchRenderer, sigc::mem_fun(*this, &PlanSheet::on_label_data));

	get_selection()->set_mode(Gtk::SELECTION_SINGLE);
	
	m_BranchRenderer.signal_edited().connect(sigc::mem_fun(*this, &PlanSheet::BranchEdited));
	m_BranchRenderer.property_editable() = true;
}

PlanSheet::~PlanSheet()
{
}

void PlanSheet::add_empty_line()
{
	Gtk::Dialog dialog(_("Choose branch:"));
	Glib::RefPtr<ORM::Data> data = ORM::Data::create(m_ColumnRecord);
	DB::DataBase::Instance().ListNewBranchForSpeciality(data, m_IdSpeciality);
	Gtk::ComboBox combo(data);
	combo.pack_start(m_ColumnRecord.fText);
	dialog.get_vbox()->pack_start(combo);
	dialog.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.show_all_children();
	dialog.show();
	if(dialog.run() == Gtk::RESPONSE_OK)
	{
		Gtk::TreeIter iter = combo.get_active();
		if(iter)
		{
			DB::DataBase::Instance().AppendNewBranchForSpeciality(m_IdSpeciality, iter->get_value(m_ColumnRecord.fId));
		}
	}
}

void PlanSheet::remove_line()
{
}

void PlanSheet::update_model()
{
	Glib::RefPtr<ORM::Data> data = ORM::Data::create(m_ColumnRecord);
	DB::DataBase::Instance().ListEntitiesText(DB::g_ModelLessonType, DB::g_ModelLessonType.abbr, data);
	set_horz_model(data);

	data = ORM::Data::create(m_ColumnRecord);
	DB::DataBase::Instance().GetTeachingBranch(data, m_IdSpeciality);
	set_vert_model(data);
}

void PlanSheet::set_speciality(long int id_speciality)
{
	m_IdSpeciality = id_speciality;
}

void PlanSheet::BranchEdited(const Glib::ustring& path, long int id)
{
}

void PlanSheet::on_label_data(Gtk::CellRenderer* cell, const Gtk::TreeModel::iterator& iter)
{
	m_BranchRenderer.property_text() = iter->get_value(m_ColumnRecord.fText);
}

void PlanSheet::on_label_button_release(const Gtk::TreeIter& iter, GdkEventButton* event)
{
	
}

