#ifndef _TABLE_CELL_H_
#define _TABLE_CELL_H_

#include <gtkmm/drawingarea.h>

const size_t CELL_WIDTH = 60;
const size_t CELL_HEIGHT = 60;

class TableCell : public Gtk::DrawingArea
{
public:
	TableCell()
		:m_Set(false)
	{
		signal_expose_event().connect(sigc::mem_fun(*this, &TableCell::OnExpose));
		set_size_request(CELL_WIDTH, CELL_HEIGHT);
	}
	TableCell(const TableCell& cell)
		:m_Teacher(cell.m_Teacher),
		m_Lesson(cell.m_Lesson),
		m_Groups(cell.m_Groups),
		m_Set(cell.m_Set)
	{
		signal_expose_event().connect(sigc::mem_fun(*this, &TableCell::OnExpose));
		set_size_request(CELL_WIDTH, CELL_HEIGHT);
	}
	TableCell(const Glib::ustring& teacher, const Glib::ustring& lesson, const Glib::ustring& groups);
	~TableCell() {}
	TableCell& operator=(const TableCell& cell)
	{
		this->~TableCell();
		new (this) TableCell(cell);
		return *this;
	}
private:
	bool OnExpose(GdkEventExpose* event);

	Glib::ustring m_Teacher, m_Lesson, m_Groups;
	bool m_Set;
};

#endif

