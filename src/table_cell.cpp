#include "table_cell.h"
#include "table_frame.h"

TableCell::TableCell(TableFrame &table_frame, size_t x, size_t y, const Glib::ustring& teacher, const Glib::ustring& lesson, const Glib::ustring& groups, size_t hours)
	:m_Teacher(teacher),
	m_Lesson(lesson),
	m_Groups(groups),
	m_Hours(hours),
	m_TableFrame(table_frame),
	m_PosX(x),
	m_PosY(y)
{
	signal_expose_event().connect(sigc::mem_fun(*this, &TableCell::OnExpose));
	signal_button_press_event().connect(sigc::mem_fun(*this, &TableCell::OnButtonEvent));
	set_events(Gdk::BUTTON_PRESS_MASK);
	set_size_request(CELL_WIDTH, CELL_HEIGHT);
}

bool TableCell::OnExpose(GdkEventExpose* event)
{
	Glib::RefPtr<Gdk::Window> window = get_window();
	if(window)
	{
		Gtk::Allocation allocation = get_allocation();
		const size_t width = allocation.get_width();
		const size_t height = allocation.get_height();
		Cairo::RefPtr<Cairo::Context> context = window->create_cairo_context();
		if(context)
		{
			context->rectangle(0, 0, CELL_WIDTH, CELL_HEIGHT);
			context->set_source_rgb(1.0, 1.0, 1.0);
			context->fill();
			context->rectangle(0, 0, CELL_WIDTH, CELL_HEIGHT);
			context->set_source_rgb(0.0, 0.0, 0.0);
			context->stroke();
			if(m_Hours)
			{
				// draw data

				context->set_source_rgb(1.0, 0.0, 0.0);
				context->move_to(2, 10);
				context->show_text(m_Teacher);
				context->stroke();

				context->set_source_rgb(0.0, 1.0, 0.0);
				context->move_to(2, 30);
				context->show_text(m_Lesson);
				context->stroke();

				context->set_source_rgb(0.0, 0.0, 1.0);
				context->move_to(2, 50);
				context->show_text(m_Groups);
				context->stroke();
				
				if(m_Hours > 1)
				{
					context->set_source_rgb(0.0, 0.0, 0.0);
					context->move_to(105, 50);
					context->show_text(Glib::ustring::format(m_Hours));
					context->stroke();
				}
			}
			else
			{
				context->move_to(10, 30);
				context->line_to(110, 30);
				context->stroke();
			}
		}
	}
	return true;
}

bool TableCell::OnButtonEvent(GdkEventButton* event)
{
	if((event->type == GDK_BUTTON_PRESS) && (event->button == 3))
	{
		m_TableFrame.SetCell(this);
		m_TableFrame.GetPopupMenu().popup(event->button, event->time);
		return true;
	}
	else
	{
		return false;
	}
}

void TableCell::ReDraw()
{
	Glib::RefPtr<Gdk::Window> window = get_window();
	if(window)
	{
		Gdk::Rectangle rect(0, 0, get_allocation().get_width(), get_allocation().get_height());
		window->invalidate_rect(rect, false);
	}
}

