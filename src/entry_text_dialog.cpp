#include <gtkmm/stock.h>
#include "entry_text_dialog.h"

void EntryTextDialog::Init()
{
	Gtk::VBox &vbox = *get_vbox();
	add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
	add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	vbox.pack_start(m_Entry);
	show_all_children();
}

