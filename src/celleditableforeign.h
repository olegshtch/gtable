#ifndef _CELL_EDITABLE_FOREIGN_H_
#define _CELL_EDITABLE_FOREIGN_H_

#include <gtkmm/celleditable.h>
#include <gtkmm/eventbox.h>
#include "orm/table.h"

class CellEditableForeign : public Gtk::EventBox, public Gtk::CellEditable
{
public:
	CellEditableForeign(const Glib::ustring& path, const ORM::Table& table, const ORM::Field<Glib::ustring>& field)
		:Glib::ObjectBase(typeid(CellEditableForeign)),
		m_Path(path),
		m_Table(table),
		m_Field(field)
	{

		show_all_children();
	}
	virtual ~CellEditableForeign()
	{
	}

	Glib::ustring get_path() const
	{
		return m_Path;
	}
	Glib::ustring get_text() const;
	long get_id() const;
	void set_id(long id);
protected:
	Glib::ustring m_Path;
	long m_Id;
	const ORM::Table& m_Table;
	const ORM::Field<Glib::ustring>& m_Field;
private:
};

#endif


