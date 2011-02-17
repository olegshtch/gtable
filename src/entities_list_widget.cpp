#include <iostream>
#include "entities_list_widget.h"
#include "db/models.h"
#include "shared.h"
#include "entry_text_dialog.h"

EntitiesListWidget::EntitiesListWidget(DB::DataBase &db, const DB::Entity &ent)
	:m_DB(db), m_Entity(ent), m_Model(ORM::Data::create(DB::g_ModelEntity)), m_ButtonBox(Gtk::BUTTONBOX_SPREAD), m_ListBox(m_Model)
{
	m_ListBox.set_headers_visible(true);
	if(&ent == &DB::g_Auditoriums)
	{
		m_Model = ORM::Data::create(DB::g_ModelAud);
		m_ListBox.set_model(m_Model);
		m_ListBox.append_column("id", DB::g_ModelAud.id);
		m_ListBox.append_column_editable("name", DB::g_ModelAud.name);
		m_ListBox.append_column_editable("multithr", DB::g_ModelAud.multithread);
	}
	else
	{
		m_ListBox.append_column("id", DB::g_ModelEntity.id);
		m_ListBox.append_column_editable("name", DB::g_ModelEntity.name);		
	}

/*	Glib::RefPtr<Gtk::TreeModel> model = m_ListBox.get_model();
	Glib::RefPtr<Gtk::ListStore> list_model = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(model);
	list_model->set_column_types(s_Model);*/

	m_Buttons[0].set_label(_("Append"));
	m_ButtonBox.pack_start(m_Buttons[0], Gtk::PACK_SHRINK);
	m_Buttons[0].signal_clicked().connect(sigc::mem_fun(*this, &EntitiesListWidget::OnAppend));

	m_Buttons[1].set_label(_("Delete"));
	m_ButtonBox.pack_start(m_Buttons[1], Gtk::PACK_SHRINK);
	m_Buttons[1].signal_clicked().connect(sigc::mem_fun(*this, &EntitiesListWidget::OnDelete));

	m_Buttons[2].set_label(_("Edit"));
	m_ButtonBox.pack_start(m_Buttons[2], Gtk::PACK_SHRINK);
	m_Buttons[2].signal_clicked().connect(sigc::mem_fun(*this, &EntitiesListWidget::OnEdit));

	m_Scrolled.add(m_ListBox);
	m_Scrolled.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	pack_start(m_Scrolled, Gtk::PACK_EXPAND_WIDGET);
	pack_end(m_ButtonBox, Gtk::PACK_SHRINK);

	Refresh();

	show_all_children();

	m_Model->signal_row_changed().connect(sigc::mem_fun(*this, &EntitiesListWidget::OnRowEdited));
	m_IsEditing = true;
}

EntitiesListWidget::~EntitiesListWidget()
{
}

void EntitiesListWidget::Refresh()
{
	m_IsEditing = false;
	if(&m_Entity == &DB::g_Auditoriums)
	{
		m_DB.ListEntityAud(m_Entity, m_Model);
	}
	else
	{
		m_DB.ListEntity(m_Entity, m_Model);
	}
	show_all_children();
	m_IsEditing = true;
}

void EntitiesListWidget::OnAppend()
{
	EntryTextDialog dialog;
	if(dialog.run() == Gtk::RESPONSE_OK)
	{
		m_DB.AppendEntity(m_Entity, dialog.GetText());
		Refresh();
	}
}

void EntitiesListWidget::OnDelete()
{
	/*Glib::ListHandle<Gtk::TreeModel::Path, Gtk::TreePath_Traits>*/
	Gtk::TreeSelection::ListHandle_Path selected = m_ListBox.get_selection()->get_selected_rows();
	for(Gtk::TreeSelection::ListHandle_Path::const_iterator it = selected.begin(); it != selected.end(); it ++)
	{
		m_DB.DeleteEntity(m_Entity, m_Model->get_iter(*it)->get_value(DB::g_ModelEntity.id));
	}
	Refresh();
}

void EntitiesListWidget::OnEdit()
{
	if(m_ListBox.get_selection()->count_selected_rows() == 1)
	{
		EntryTextDialog dialog;
		dialog.SetText(m_ListBox.get_selection()->get_selected()->get_value(DB::g_ModelEntity.name));
		if(dialog.run() == Gtk::RESPONSE_OK)
		{
			m_DB.EditEntityName(m_Entity, m_ListBox.get_selection()->get_selected()->get_value(DB::g_ModelEntity.id), dialog.GetText());
			Refresh();
		}
	}
}

void EntitiesListWidget::OnRowEdited(const Gtk::TreeModel::Path& path, const Gtk::TreeModel::iterator& iter)
{
	if(m_IsEditing)
	{
		m_DB.EditEntityName(m_Entity, iter->get_value(DB::g_ModelEntity.id), iter->get_value(DB::g_ModelEntity.name));
		if(&m_Entity == &DB::g_Auditoriums)
		{
			m_DB.EditMultithr(m_Entity, iter->get_value(DB::g_ModelEntity.id), iter->get_value(DB::g_ModelAud.multithread));
		}
	}
}

