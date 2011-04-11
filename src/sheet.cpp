#include <iostream>
#include "sheet.h"

Sheet::Sheet()
	:Glib::ObjectBase(typeid(Sheet)),
	m_refVertModel(ORM::Data::create(m_ColumnRecord)),
	m_refHorzModel(ORM::Data::create(m_ColumnRecord))
{
	Init();
}

Sheet::Sheet(GtkTreeView *cobject, const Glib::RefPtr<Gtk::Builder>& builder)
	:Glib::ObjectBase(typeid(Sheet)),
	Gtk::TreeView(cobject),
	m_refVertModel(ORM::Data::create(m_ColumnRecord)),
	m_refHorzModel(ORM::Data::create(m_ColumnRecord))
{
	Init();
}

Sheet::~Sheet()
{
}

void Sheet::set_horz_model(const Glib::RefPtr<ORM::Data>& horz_model)
{
	m_refHorzModel = horz_model;
	set_columns_count(horz_model->children().size());
	for(size_t i = 0; i < horz_model->children().size(); ++ i)
	{
		get_column(i + 1)->set_title(horz_model->children()[i].get_value(m_ColumnRecord.fText));
	}
}

void Sheet::set_vert_model(const Glib::RefPtr<ORM::Data>& vert_model)
{
	m_refVertModel = vert_model;
	m_RowsCount = m_refVertModel->children().size();
	set_model(m_refVertModel);
}

void Sheet::on_label_data(Gtk::CellRenderer* cell, const Gtk::TreeModel::iterator& iter)
{
	//std::cout << "Sheet::on_label_data" << std::endl;
	m_LabelRenderer.property_text() = iter->get_value(m_ColumnRecord.fText);
}

void Sheet::on_cell_data(Gtk::CellRenderer* cell, const Gtk::TreeModel::iterator& iter, long int column_id)
{
	//std::cout << "Sheet::on_cell_data" << std::endl;
	long row_id = iter->get_value(m_ColumnRecord.fId);
	//m_CellRenderer.property_text() = Glib::ustring::format(row_id, ":", column_id);
	signal_cell_data_.emit(cell, row_id, column_id);
}

void Sheet::Init()
{
	set_model(m_refVertModel);
	insert_column_with_data_func(0, Glib::ustring(), m_LabelRenderer, sigc::mem_fun(*this, &Sheet::on_label_data));
	property_can_focus() = true;
	set_headers_visible(true);
	set_grid_lines(Gtk::TREE_VIEW_GRID_LINES_BOTH);
	set_headers_clickable(true);
	set_enable_search(false);

	m_LabelRenderer.property_background_gdk() = Gdk::Color("Gray");
	
	show_all_children();
}

void Sheet::set_columns_count(size_t columns)
{
	m_ColumnsCount = columns;

	if(get_columns().size() != columns + 1)
	{
		long delta = get_columns().size() - columns - 1;
		if(delta < 0)
		{
			//append rows
			for(; delta != 0; ++ delta)
			{
				Gtk::TreeViewColumn *tree_view_column = new Gtk::TreeViewColumn(Glib::ustring(), m_CellRenderer);
				tree_view_column->set_cell_data_func(m_CellRenderer, sigc::bind(sigc::mem_fun(*this, &Sheet::on_cell_data), m_refHorzModel->children()[static_cast<long>(columns) + delta].get_value(m_ColumnRecord.fId)));
				append_column(*Gtk::manage(tree_view_column));
			}
		}
		else
		{
			//remove rows
			for(; delta != 0; -- delta)
			{
				remove_column(*get_column(get_columns().size() - 1));
			}
		}
	}
}

