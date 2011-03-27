#include <iostream>
#include "cellrendererforeign.h"

void CellRendererForeign::get_size_vfunc(Gtk::Widget& widget, const Gdk::Rectangle* cell_area, int* x_offset, int* y_offset, int* width, int* height) const
{
	if(cell_area)
	{
		std::cout << "CellRendererForeign::get_size_vfunc" << std::endl;
		(*x_offset) = cell_area->get_x();
		(*y_offset) = cell_area->get_y();
		(*width) = cell_area->get_width();
		(*height) = cell_area->get_height();
	}
}

void CellRendererForeign::render_vfunc(const Glib::RefPtr<Gdk::Drawable>& window, Gtk::Widget& widget, const Gdk::Rectangle& background_area, const Gdk::Rectangle& cell_area, const Gdk::Rectangle& expose_area, Gtk::CellRendererState flags)
{
	std::cout << "CellRendererForeign::render_vfunc" << std::endl;
	
}

bool CellRendererForeign::activate_vfunc(GdkEvent* event, Gtk::Widget& widget, const Glib::ustring& path, const Gdk::Rectangle& background_area, const Gdk::Rectangle& cell_area, Gtk::CellRendererState flags)
{
	std::cout << "CellRendererForeign::activate_vfunc" << std::endl;
	return true;
}

Gtk::CellEditable* CellRendererForeign::start_editing_vfunc(GdkEvent* event, Gtk::Widget& widget, const Glib::ustring& path, const Gdk::Rectangle& background_area, const Gdk::Rectangle& cell_area, Gtk::CellRendererState flags)
{
	std::cout << "CellRendererForeign::start_editing_vfunc" << std::endl;
	return 0;
}


