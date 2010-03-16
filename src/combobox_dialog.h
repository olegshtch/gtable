#ifndef _LINK_CHOOSE_DIALOG_H_
#define _LINK_CHOOSE_DIALOG_H_

#include <gtkmm/dialog.h>
#include <gtkmm/combobox.h>
#include <gtkmm/liststore.h>

class ComboboxDialog : public Gtk::Dialog
{
public:
	ComboboxDialog(Glib::RefPtr<Gtk::ListStore> &model);
	~ComboboxDialog() {}
	Gtk::TreeModel::iterator GetRow() const {return m_ComboBox.get_active();}
	template<class ColumnType> void PackStart(const Gtk::TreeModelColumn<ColumnType>& model_column, bool expand=true)
	{
		m_ComboBox.pack_start(model_column, expand);
	}
private:
	Glib::RefPtr<Gtk::ListStore> m_Model;
	Gtk::ComboBox m_ComboBox;
};

#endif

