#ifndef _CELL_RENDERER_FOREIGN_H_
#define _CELL_RENDERER_FOREIGN_H_

#include <gtkmm/cellrenderer.h>
#include <gtkmm/treemodel.h>
#include "celleditableforeign.h"

class CellRendererForeign : public Gtk::CellRenderer
{
public:
	CellRendererForeign(const Glib::RefPtr<Gtk::TreeModel>& model)
		:Glib::ObjectBase(typeid(CellRendererForeign)),
		property_text_(*this, "text", ""),
		property_id_(*this, "id", 0),
		property_editable_(*this, "editable", true),
		cell_editable_foreign_(NULL)
	{
	}
	virtual ~CellRendererForeign()
	{
	}

	Glib::PropertyProxy<Glib::ustring> property_text()
	{
		return property_text_.get_proxy();
	}

	Glib::PropertyProxy<long> property_id()
	{
		return property_id_.get_proxy();
	}

	Glib::PropertyProxy<bool> property_editable()
	{
		return property_editable_.get_proxy();
	}

	typedef sigc::signal<void, const Glib::ustring&, const Glib::ustring&> signal_edited_t;
	signal_edited_t& signal_edited()
	{
		return signal_edited_;
	}
protected:
	Glib::Property<Glib::ustring> property_text_;
	Glib::Property<long> property_id_;
	Glib::Property<bool> property_editable_;
	signal_edited_t signal_edited_;
	CellEditableForeign* cell_editable_foreign_;

	virtual void get_size_vfunc(Gtk::Widget& widget, const Gdk::Rectangle* cell_area, int* x_offset, int* y_offset, int* width, int* height) const;
	virtual void render_vfunc(const Glib::RefPtr<Gdk::Drawable>& window, Gtk::Widget& widget, const Gdk::Rectangle& background_area, const Gdk::Rectangle& cell_area, const Gdk::Rectangle& expose_area, Gtk::CellRendererState flags);
	virtual bool activate_vfunc(GdkEvent* event, Gtk::Widget& widget, const Glib::ustring& path, const Gdk::Rectangle& background_area, const Gdk::Rectangle& cell_area, Gtk::CellRendererState flags);
	virtual Gtk::CellEditable* start_editing_vfunc(GdkEvent* event, Gtk::Widget& widget, const Glib::ustring& path, const Gdk::Rectangle& background_area, const Gdk::Rectangle& cell_area, Gtk::CellRendererState flags);
private:

};

#endif

