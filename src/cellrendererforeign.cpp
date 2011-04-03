#include <iostream>
#include "db/db.h"
#include "cellrendererforeign.h"

Gtk::CellEditable* CellRendererForeign::start_editing_vfunc(GdkEvent* event, Gtk::Widget& widget, const Glib::ustring& path, const Gdk::Rectangle& background_area, const Gdk::Rectangle& cell_area, Gtk::CellRendererState flags)
{
	std::cout << "CellRendererForeign::start_editing_vfunc" << std::endl;
	DB::DataBase::Instance().ListEntitiesText(m_ForeignTable, m_ForeignField, m_ComboData);
	m_Editable = Gtk::manage(new CellEditableForeign(path, m_ComboData, m_Scheme.fText));
	m_Editable->signal_editing_done().connect(sigc::mem_fun(*this, &CellRendererForeign::on_editing_done));
	m_Editable->show();

	return m_Editable;
}

void CellRendererForeign::ForeignKeyAsString(Gtk::CellRenderer *cell, const Gtk::TreeModel::iterator& row)
{
	CellRendererForeign *p_renderer = dynamic_cast<CellRendererForeign*>(cell);
	long id = row->get_value(p_renderer->m_Field);
	p_renderer->property_text() = DB::DataBase::Instance().GetTextById(p_renderer->m_ForeignTable, p_renderer->m_ForeignField, id);
}

void CellRendererForeign::on_editing_done()
{
	std::cout << "CellRendererForeign::on_editing_done" << std::endl;
	std::cout << "\tid=" << m_Editable->get_iter()->get_value(m_Scheme.fId) << std::endl;
	std::cout << "\tpath=" << m_Editable->get_path() << std::endl;
	signal_edited_.emit(m_Editable->get_path(), m_Editable->get_iter()->get_value(m_Scheme.fId));
}

