#ifndef _TABLE_CELL_H_
#define _TABLE_CELL_H_

#include <gtkmm/drawingarea.h>

const size_t CELL_WIDTH = 120;
const size_t CELL_HEIGHT = 60;

class TableCell : public Gtk::DrawingArea
{
public:
/*	TableCell()
		:m_Hours(0)
	{
		signal_expose_event().connect(sigc::mem_fun(*this, &TableCell::OnExpose));
		set_size_request(CELL_WIDTH, CELL_HEIGHT);
	}*/
	TableCell(const TableCell& cell)
		:m_Teacher(cell.m_Teacher),
		m_Lesson(cell.m_Lesson),
		m_Groups(cell.m_Groups),
		m_Hours(cell.m_Hours)
	{
		signal_expose_event().connect(sigc::mem_fun(*this, &TableCell::OnExpose));
		set_size_request(CELL_WIDTH, CELL_HEIGHT);
	}
	TableCell(const Glib::ustring& teacher = "", const Glib::ustring& lesson = "", const Glib::ustring& groups = "", size_t hours = 0);
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
	size_t m_Hours;
};

#endif

