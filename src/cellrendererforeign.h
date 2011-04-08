#ifndef _CELL_RENDERER_FOREIGN_H_
#define _CELL_RENDERER_FOREIGN_H_

#include <gtkmm/cellrenderercombo.h>
#include <gtkmm/treemodel.h>
#include "celleditableforeign.h"
#include "orm/foreign_key.h"
#include "orm/scheme.h"
#include "orm/data.h"

class CellRendererForeign : public Gtk::CellRendererCombo
{
	class ComboScheme : public ORM::Scheme
	{
	public:
		ORM::Field<long int> fId;
		ORM::Field<Glib::ustring> fText;

		ComboScheme()
			:fId(""),
			fText("")
		{
			add(fId);
			add(fText);
		}
	};
public:
	CellRendererForeign(const ORM::Field<ORM::ForeignKey>& field, const ORM::Table& foreign_table, const ORM::Field<Glib::ustring>& foreign_field)
		:Glib::ObjectBase(typeid(CellRendererForeign)),
		m_ComboData(ORM::Data::create(m_Scheme)),
		m_Field(field),
		m_ForeignTable(foreign_table),
		m_ForeignField(foreign_field),
		m_Editable(NULL)
	{
		property_editable() = true;
		property_model() = m_ComboData;
		property_text_column() = m_Scheme.fText.index();
	}
	virtual ~CellRendererForeign()
	{
	}

	static void ForeignKeyAsString(Gtk::CellRenderer *cell, const Gtk::TreeModel::iterator& row);

	// Signal for editing done
	typedef sigc::signal<void, const Glib::ustring&, long int> signal_edited_t;
	signal_edited_t& signal_edited() { return signal_edited_; };

protected:
	virtual Gtk::CellEditable* start_editing_vfunc(GdkEvent* event, Gtk::Widget& widget, const Glib::ustring& path, const Gdk::Rectangle& background_area, const Gdk::Rectangle& cell_area, Gtk::CellRendererState flags);

	signal_edited_t signal_edited_;
private:
	const ComboScheme m_Scheme;
	Glib::RefPtr<ORM::Data> m_ComboData;

	const ORM::Field<ORM::ForeignKey>& m_Field;
	const ORM::Table& m_ForeignTable;
	const ORM::Field<Glib::ustring>& m_ForeignField;
	CellEditableForeign* m_Editable;

	virtual void on_editing_done();
};

#endif
