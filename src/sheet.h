#ifndef _SHEET_H_
#define _SHEET_H_

#include <gtkmm/drawingarea.h>
#include <vector>

class Sheet : public Gtk::DrawingArea
{
public:
	Sheet();
	~Sheet();

	size_t get_rows_count() const
	{
		return m_RowsCount;
	}

	void set_rows_count(size_t rows);

	size_t get_columns_count() const
	{
		return m_ColumnsCount;
	}

	void set_columns_count(size_t columns);
	Gdk::Rectangle get_label_vert_rect(size_t index) const
	{
		return Gdk::Rectangle(0, m_HorzLabelHeight + index * m_CellHeight, m_VertLabelWidth, m_CellHeight);
	}
	Gdk::Rectangle get_label_horz_rect(size_t index) const
	{
		return Gdk::Rectangle(m_VertLabelWidth + index * m_CellWidth, 0, m_CellWidth, m_HorzLabelHeight);
	}
	Gdk::Rectangle get_cell_rect(size_t row, size_t column) const
	{
		return Gdk::Rectangle(m_VertLabelWidth + column * m_CellWidth, m_HorzLabelHeight + row * m_CellHeight, m_CellWidth, m_CellHeight);
	}

protected:
	virtual bool on_expose_event(GdkEventExpose* event);
	virtual void on_size_request(Gtk::Requisition* requisition);
private:
	//Gtk::CellRendererText *m_CellRenderer;
	std::vector<std::vector<Glib::ustring> > m_TextMatrix;
	std::vector<Glib::ustring> m_VertLabels;
	std::vector<Glib::ustring> m_HorzLabels;
	size_t m_RowsCount;
	size_t m_ColumnsCount;
	size_t m_CellWidth;
	size_t m_CellHeight;
	size_t m_VertLabelWidth;
	size_t m_HorzLabelHeight;
};

#endif

