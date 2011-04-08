#ifndef _SHEET_H_
#define _SHEET_H_

#include <gtkmm/treeview.h>
#include <gtkmm/cellrenderertext.h>
#include <gtkmm/liststore.h>
#include <vector>

class Sheet : public Gtk::TreeView
{
public:
	Sheet();
	~Sheet();

	void set_rows_count(size_t rows);
	void set_columns_count(size_t columns);

	void set_horz_label(size_t column, const Glib::ustring& label);
	void set_vert_label(size_t row, const Glib::ustring& label);
	void set_cell_data(size_t row, size_t column, const Glib::ustring& label)
	{
		m_TextMatrix[row][column] = label;
	}
protected:
	void on_label_data(Gtk::CellRenderer* cell, const Gtk::TreeModel::iterator& iter);
	void on_cell_data(Gtk::CellRenderer* cell, const Gtk::TreeModel::iterator& iter, size_t column);
private:
	class SheetModel : public Gtk::TreeModelColumnRecord
	{
	public:
		Gtk::TreeModelColumn<unsigned long> id;
		Gtk::TreeModelColumn<Glib::ustring> label;
		SheetModel()
		{
			add(id);
			add(label);
		}
	};

	SheetModel m_ColumnRecord;
	Glib::RefPtr<Gtk::ListStore> m_refModel;
	Gtk::CellRendererText m_CellRenderer;
	Gtk::CellRendererText m_LabelRenderer;
	std::vector<std::vector<Glib::ustring> > m_TextMatrix;
	size_t m_RowsCount;
	size_t m_ColumnsCount;
};

#endif

