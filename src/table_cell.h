#ifndef _TABLE_CELL_H_
#define _TABLE_CELL_H_

#include <gtkmm/drawingarea.h>

const size_t CELL_WIDTH = 120;
const size_t CELL_HEIGHT = 60;

class TableFrame;

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
		m_Hours(cell.m_Hours),
		m_TableFrame(cell.m_TableFrame),
		m_PosX(cell.m_PosX),
		m_PosY(cell.m_PosY)
	{
		signal_expose_event().connect(sigc::mem_fun(*this, &TableCell::OnExpose));
		signal_button_press_event().connect(sigc::mem_fun(*this, &TableCell::OnButtonEvent));
		set_events(Gdk::BUTTON_PRESS_MASK);
		set_size_request(CELL_WIDTH, CELL_HEIGHT);
	}
	TableCell(TableFrame &table_frame, size_t x, size_t y, const Glib::ustring& teacher = "", const Glib::ustring& lesson = "", const Glib::ustring& groups = "", size_t hours = 0);
	~TableCell() {}
	TableCell& operator=(const TableCell& cell)
	{
		this->~TableCell();
		new (this) TableCell(cell);
		return *this;
	}
	void Increment()
	{
		m_Hours ++;
	}
	void Decrement()
	{
		m_Hours --;
	}
	void ReDraw();
	Glib::ustring GetTeacher()
	{
		return m_Teacher;
	}
	Glib::ustring GetLesson()
	{
		return m_Lesson;
	}
	Glib::ustring GetGroups()
	{
		return m_Groups;
	}
private:
	bool OnExpose(GdkEventExpose* event);
	bool OnButtonEvent(GdkEventButton* event);

	Glib::ustring m_Teacher, m_Lesson, m_Groups;
	size_t m_Hours;
	TableFrame &m_TableFrame;
public:
	size_t m_PosX, m_PosY;
};

#endif

