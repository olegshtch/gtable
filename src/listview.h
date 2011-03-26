#ifndef _LISTVIEW_H_
#define _LISTVIEW_H_

#include <gtkmm/treeview.h>
#include <gtkmm/builder.h>
#include <gtkmm/cellrenderercombo.h>
#include "orm/data.h"
#include "orm/table.h"
#include "orm/foreign_key.h"
#include "db/db.h"

class ListView : public Gtk::TreeView
{
public:
	ListView(GtkTreeView *cobject, Glib::RefPtr<Gtk::Builder>&)
		:Gtk::TreeView(cobject),
		m_Scheme(NULL),
		m_Refresh(true)
	{
		set_can_focus(true);
		set_headers_visible(true);
	}
	~ListView()
	{
	}
	void set_scheme(const ORM::Table& scheme)
	{
		m_Scheme = &scheme;
		m_refModel = ORM::Data::create(scheme);
		set_model(m_refModel);
		m_refModel->signal_row_changed().connect(sigc::mem_fun(*this, &ListView::on_row_changed));
	}
	void update_model()
	{
		m_Refresh = false;
		DB::DataBase::Instance().ListEntity(*m_Scheme, m_refModel);
		m_Refresh = true;
		show_all_children();
	}
	void add_empty_line()
	{
		DB::DataBase::Instance().AppendEntity(*m_Scheme, m_refModel->append());
		update_model();
	}
	int append_column_foreign_editable(const Glib::ustring& str, const ORM::Field<ORM::ForeignKey>& field, Glib::RefPtr<Gtk::TreeModel> foreign_model, const ORM::Field<Glib::ustring>& foreign_field)
	{
		Gtk::CellRendererCombo *combo = Gtk::manage(new Gtk::CellRendererCombo());
		combo->property_has_entry() = false;
		combo->property_model() = foreign_model;
		combo->property_text_column() = foreign_field.index();
		combo->property_editable() = true;
		return append_column(str, *combo);
	}
	virtual void on_row_changed(const Gtk::TreeModel::Path& path, const Gtk::TreeModel::iterator& iter);
private:
	const ORM::Table* m_Scheme;
	Glib::RefPtr<ORM::Data> m_refModel;
	bool m_Refresh;
};

#endif

