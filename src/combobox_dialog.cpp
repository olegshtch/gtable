#include <gtkmm/stock.h>
#include "combobox_dialog.h"

ComboboxDialog::ComboboxDialog(Glib::RefPtr<Gtk::ListStore> model)
	:m_Model(model), m_ComboBox(m_Model)
{
	get_vbox()->pack_start(m_ComboBox, Gtk::PACK_EXPAND_WIDGET);
	add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
	add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	show_all_children();
}

