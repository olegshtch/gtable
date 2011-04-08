#include <iostream>
#include "sheet.h"

Sheet::Sheet()
	:Glib::ObjectBase(typeid(Sheet)),
	m_refModel(Gtk::ListStore::create(m_ColumnRecord))
{
	set_model(m_refModel);
	insert_column_with_data_func(0, Glib::ustring(), m_LabelRenderer, sigc::mem_fun(*this, &Sheet::on_label_data));
	property_can_focus() = true;
	set_headers_visible(true);
	set_grid_lines(Gtk::TREE_VIEW_GRID_LINES_BOTH);

	m_LabelRenderer.property_background_gdk() = Gdk::Color("Gray");
	
	show_all_children();
}

Sheet::~Sheet()
{
}

void Sheet::on_label_data(Gtk::CellRenderer* cell, const Gtk::TreeModel::iterator& iter)
{
	//std::cout << "Sheet::on_label_data" << std::endl;
	m_LabelRenderer.property_text() = iter->get_value(m_ColumnRecord.label);
}

void Sheet::on_cell_data(Gtk::CellRenderer* cell, const Gtk::TreeModel::iterator& iter, size_t column)
{
	//std::cout << "Sheet::on_cell_data" << std::endl;
	m_CellRenderer.property_text() = m_TextMatrix[iter->get_value(m_ColumnRecord.id)][column];
}

void Sheet::set_rows_count(size_t rows)
{
	m_RowsCount = rows;
	m_TextMatrix.resize(rows, std::vector<Glib::ustring>(m_ColumnsCount));

	if(m_refModel->children().size() != m_RowsCount)
	{
		long delta = m_refModel->children().size() - m_RowsCount;
		if(delta < 0)
		{
			//append rows
			for(; delta != 0; ++ delta)
			{
				Gtk::TreeIter iter = m_refModel->append();
				iter->set_value(m_ColumnRecord.id, m_RowsCount + delta);
			}
		}
		else
		{
			// delete rows
			for(; delta != 0; -- delta)
			{
				m_refModel->erase(-- m_refModel->children().end());
			}
		}
	}
}

void Sheet::set_columns_count(size_t columns)
{
	m_ColumnsCount = columns;
	for(std::vector<std::vector<Glib::ustring> >::iterator it = m_TextMatrix.begin(); it != m_TextMatrix.end(); ++ it)
	{
		it->resize(columns);
	}

	if(get_columns().size() != columns + 1)
	{
		long delta = get_columns().size() - columns - 1;
		if(delta < 0)
		{
			//append rows
			for(; delta != 0; ++ delta)
			{
				Gtk::TreeViewColumn *tree_view_column = new Gtk::TreeViewColumn(Glib::ustring(), m_CellRenderer);
				tree_view_column->set_cell_data_func(m_CellRenderer, sigc::bind(sigc::mem_fun(*this, &Sheet::on_cell_data), static_cast<size_t>(static_cast<long>(columns) + delta)));
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

void Sheet::set_horz_label(size_t column, const Glib::ustring& label)
{
	get_column(column + 1)->set_title(label);
}

void Sheet::set_vert_label(size_t row, const Glib::ustring& label)
{
	m_refModel->children()[row].set_value(m_ColumnRecord.label, label);
}

