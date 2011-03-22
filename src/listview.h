#ifndef _LISTVIEW_H_
#define _LISTVIEW_H_

#include <gtkmm/treeview.h>
#include <gtkmm/builder.h>
#include "orm/data.h"
#include "orm/table.h"
#include "db/db.h"

class ListView : public Gtk::TreeView
{
public:
	ListView(GtkTreeView *cobject, Glib::RefPtr<Gtk::Builder>&)
		:Gtk::TreeView(cobject),
		m_Scheme(NULL)
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
	}
	void update_model()
	{
		DB::DataBase::Instance().ListEntity(*m_Scheme, m_refModel);
	}
	void add_empty_line()
	{
	}
private:
	const ORM::Table* m_Scheme;
	Glib::RefPtr<ORM::Data> m_refModel;
};

#endif

