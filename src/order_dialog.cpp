#include <gtkmm/stock.h>
#include "order_dialog.h"
#include "db/models.h"
#include "shared.h"
#include "combobox_dialog.h"

OrderDialog::OrderDialog(DB::DataBase &db, const DB::Link_N2N &link)
	:m_DB(db), m_Link(link), m_Model(ORM::Data::create(DB::g_ModelOrder)), m_ButtonBox(Gtk::BUTTONBOX_SPREAD), m_List(m_Model)
{
	m_Buttons[0].set_label(_("Append"));
	m_Buttons[0].signal_clicked().connect(sigc::mem_fun(*this, &OrderDialog::OnAppend));
	m_Buttons[1].set_label(_("Delete"));
	m_Buttons[1].signal_clicked().connect(sigc::mem_fun(*this, &OrderDialog::OnDelete));

	m_ButtonBox.pack_start(m_Buttons[0]);
	m_ButtonBox.pack_start(m_Buttons[1]);
	get_vbox()->pack_start(m_List, Gtk::PACK_EXPAND_WIDGET);
	get_vbox()->pack_start(m_ButtonBox, Gtk::PACK_SHRINK);

	m_List.set_headers_visible(true);
	m_List.append_column("g_id", DB::g_ModelOrder.id);
	m_List.append_column(_("Greater"), DB::g_ModelOrder.name);
	m_List.append_column("l_id", DB::g_ModelOrder.l_id);
	m_List.append_column(_("Less"), DB::g_ModelOrder.l_name);

	add_button(Gtk::Stock::CLOSE, Gtk::RESPONSE_NONE);
	Refresh();
}

OrderDialog::~OrderDialog()
{
}

void OrderDialog::OnAppend()
{
	Glib::RefPtr<ORM::Data> model = ORM::Data::create(DB::g_ModelEntity);
	m_DB.ListEntity(m_Link.m_Entity1, model);
	ComboboxDialog dialog(model);
	dialog.PackStart(DB::g_ModelEntity.name);
	if(dialog.run() == Gtk::RESPONSE_OK)
	{
		long parent_id = dialog.GetRow()->get_value(DB::g_ModelEntity.id);
		if(dialog.run() == Gtk::RESPONSE_OK)
		{
			m_DB.AppendLinkedEntity(m_Link, parent_id, dialog.GetRow()->get_value(DB::g_ModelEntity.id));
			Refresh();
		}
	}
}

void OrderDialog::OnDelete()
{
	Gtk::TreeSelection::ListHandle_Path selected = m_List.get_selection()->get_selected_rows();
	for(Gtk::TreeSelection::ListHandle_Path::const_iterator it = selected.begin(); it != selected.end(); it ++)
	{
		m_DB.DeleteLinkedEntity(m_Link,  m_Model->get_iter(*it)->get_value(DB::g_ModelOrder.id), m_Model->get_iter(*it)->get_value(DB::g_ModelOrder.l_id));
	}
	Refresh();
}

void OrderDialog::Refresh()
{
	m_DB.ListLink(m_Link, m_Model);
	show_all_children();
}


