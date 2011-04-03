#include <iostream>
#include "cellrendererforeign.h"
#include "listview.h"

void ListView::on_row_changed(const Gtk::TreeModel::Path& path, const Gtk::TreeModel::iterator& iter)
{
	if(m_Refresh)
	{
		std::cout << "update row " << path.to_string() << std::endl;
		DB::DataBase::Instance().EditEntity(*m_Scheme, iter);
	}
}

int ListView::append_column_foreign_editable(const Glib::ustring& str, const ORM::Field<ORM::ForeignKey>& field, const ORM::Table& foreign_table, const ORM::Field<Glib::ustring>& foreign_field)
{
	Gtk::TreeViewColumn *p_column = Gtk::manage(new Gtk::TreeViewColumn(str));
	CellRendererForeign *p_renderer = Gtk::manage(new CellRendererForeign(field, foreign_table, foreign_field));
	p_column->pack_start(*p_renderer);
	p_renderer->signal_edited().connect(sigc::bind(sigc::mem_fun(*this, &ListView::OnForeignEdited), &field));

	Gtk::TreeViewColumn::SlotCellData slot = sigc::ptr_fun(CellRendererForeign::ForeignKeyAsString);

	p_column->set_cell_data_func(*p_renderer, slot);

	return append_column(*p_column);
}

void ListView::OnForeignEdited(const Glib::ustring& path, long int id, const ORM::Field<ORM::ForeignKey> *field)
{
	m_refModel->get_iter(path)->set_value(*field, id);
}

