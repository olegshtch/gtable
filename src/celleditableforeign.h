#ifndef _CELL_EDITABLE_FOREIGN_H_
#define _CELL_EDITABLE_FOREIGN_H_

#include <gtkmm/celleditable.h>
#include <gtkmm/eventbox.h>
#include <gtkmm/combobox.h>
#include "orm/data.h"

class CellEditableForeign : public Gtk::EventBox, public Gtk::CellEditable
{
public:
	CellEditableForeign(const Glib::ustring& path, const Glib::RefPtr<ORM::Data>& data, const ORM::Field<Glib::ustring>& field)
		:Glib::ObjectBase(typeid(CellEditableForeign)),
		m_Path(path),
		m_Data(data),
		m_Field(field),
		m_ComboBox(m_Data)
	{
		add(m_ComboBox);
		m_ComboBox.pack_start(field);
		m_ComboBox.popup();

		set_flags(Gtk::CAN_FOCUS);

		show_all_children();
	}
	virtual ~CellEditableForeign()
	{
	}

	Glib::ustring get_path() const
	{
		return m_Path;
	}
	Gtk::TreeIter get_iter() const
	{
		return m_ComboBox.get_active();
	}
	void set_iter(const Gtk::TreeIter& iter)
	{
		m_ComboBox.set_active(iter);
	}

	// Signal for editing done
	typedef sigc::signal<void> signal_editing_done_t;
	signal_editing_done_t& signal_editing_done() { return signal_editing_done_; };
protected:
	Glib::ustring m_Path;
	Glib::RefPtr<ORM::Data> m_Data;
	const ORM::Field<Glib::ustring>& m_Field;
	signal_editing_done_t signal_editing_done_;

	// Override CellEditable
	virtual void start_editing_vfunc(GdkEvent* event);
private:
	Gtk::ComboBox m_ComboBox;
	void OnComboBoxChanged();
};

#endif


