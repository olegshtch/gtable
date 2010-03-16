#include "link_list_widget.h"
#include "db/models.h"
#include "shared.h"
#include "combobox_dialog.h"

LinkListWidget::LinkListWidget(DB::DataBase &db, const DB::Link_N2N &link)
	:m_DB(db), m_Link(link), m_ParentId(-1), m_Model(Gtk::ListStore::create(DB::g_ModelEntity)), m_ButtonBox(Gtk::BUTTONBOX_SPREAD), m_ListBox(m_Model)
{
	m_ListBox.set_headers_visible(true);
	m_ListBox.append_column("id", DB::g_ModelEntity.id);
	m_ListBox.append_column_editable("name", DB::g_ModelEntity.name);		
	
/*	Glib::RefPtr<Gtk::TreeModel> model = m_ListBox.get_model();
	Glib::RefPtr<Gtk::ListStore> list_model = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(model);
	list_model->set_column_types(s_Model);*/

	m_Buttons[0].set_label(_("Append"));
	m_ButtonBox.pack_start(m_Buttons[0], Gtk::PACK_SHRINK);
	m_Buttons[0].signal_clicked().connect(sigc::mem_fun(*this, &LinkListWidget::OnAppend));

	m_Buttons[1].set_label(_("Delete"));
	m_ButtonBox.pack_start(m_Buttons[1], Gtk::PACK_SHRINK);
	m_Buttons[1].signal_clicked().connect(sigc::mem_fun(*this, &LinkListWidget::OnDelete));

	m_Scrolled.add(m_ListBox);
	m_Scrolled.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	pack_start(m_Scrolled, Gtk::PACK_EXPAND_WIDGET);
	pack_end(m_ButtonBox, Gtk::PACK_SHRINK);

	Refresh();

	show_all_children();
}

LinkListWidget::~LinkListWidget()
{
}

void LinkListWidget::Refresh()
{
	m_DB.ListLinkedEntity(m_Link, m_ParentId, m_Model);
	show_all_children();
}

void LinkListWidget::OnAppend()
{
	Glib::RefPtr<Gtk::ListStore> model = Gtk::ListStore::create(DB::g_ModelEntity);
	m_DB.ListEntity(m_Link.m_Entity2, model);
	ComboboxDialog dialog(model);
	dialog.PackStart(DB::g_ModelEntity.name);
	if(dialog.run() == Gtk::RESPONSE_OK)
	{
		m_DB.AppendLinkedEntity(m_Link, m_ParentId, dialog.GetRow()->get_value(DB::g_ModelEntity.id));
		Refresh();
	}
}

void LinkListWidget::OnDelete()
{
	Gtk::TreeSelection::ListHandle_Path selected = m_ListBox.get_selection()->get_selected_rows();
	for(Gtk::TreeSelection::ListHandle_Path::const_iterator it = selected.begin(); it != selected.end(); it ++)
	{
		m_DB.DeleteLinkedEntity(m_Link, m_ParentId, m_Model->get_iter(*it)->get_value(DB::g_ModelEntity.id));
	}
	Refresh();
}

