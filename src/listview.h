#ifndef _LISTVIEW_H_
#define _LISTVIEW_H_

#include <gtkmm/treeview.h>
#include <gtkmm/builder.h>
#include "orm/data.h"
#include "orm/scheme.h"

class ListView : public Gtk::TreeView
{
public:
	ListView(GtkTreeView *cobject, Glib::RefPtr<Gtk::Builder>&)
		:Gtk::TreeView(cobject)
	{
		set_can_focus(true);
		set_headers_visible(true);
	}
	~ListView()
	{
	}
	void set_scheme(const ORM::Scheme& scheme)
	{
		m_refModel = ORM::Data::create(scheme);
		set_model(m_refModel);
	}
	void update_model()
	{
		
	}
private:
	Glib::RefPtr<ORM::Data> m_refModel;
};

#endif

