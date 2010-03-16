#ifndef _ENTRY_TEXT_DIALOG_H_
#define _ENTRY_TEXT_DIALOG_H_

#include <gtkmm/dialog.h>
#include <gtkmm/entry.h>

class EntryTextDialog : public Gtk::Dialog
{
public:
	EntryTextDialog(const Glib::ustring& title, Gtk::Window& parent, bool modal=false, bool use_separator=false)
		:Gtk::Dialog(title, parent, modal, use_separator) {Init();}
	EntryTextDialog(const Glib::ustring& title, bool modal = false, bool use_separator = false)
		:Gtk::Dialog(title, modal, use_separator) {Init();}
	EntryTextDialog()
		:Gtk::Dialog() {Init();}
	~EntryTextDialog() {}
	Glib::ustring GetText() {return m_Entry.get_text();}
	void SetText(const Glib::ustring& text) {m_Entry.set_text(text);}
private:
	void Init();
	Gtk::Entry m_Entry;
};

#endif

