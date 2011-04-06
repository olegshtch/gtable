#include <iostream>
#include "sheet.h"

Sheet::Sheet()
	:Glib::ObjectBase(typeid(Sheet))
{
	set_rows_count(2);
	set_columns_count(2);
	m_CellWidth = 100;
	m_CellHeight = 75;
	m_VertLabelWidth = 20;
	m_HorzLabelHeight = 20;
	m_TextMatrix[0][0] = "123";
	m_TextMatrix[0][1] = "1afasdfa";
	m_TextMatrix[1][0] = "adfvxc asdf";
	m_TextMatrix[1][1] = "1dfasf adsf adf sf";
	m_VertLabels[0] = "12";
	m_VertLabels[1] = "2";
	m_HorzLabels[0] = "asd";
	m_HorzLabels[1] = "hkhj";
}

bool Sheet::on_expose_event(GdkEventExpose* event)
{
	//std::cout << "Sheet: expose event" << std::endl;
	Glib::RefPtr<Gdk::Window> ref_window = get_window();
	Gdk::Rectangle expose_rect(&event->area);
	if(ref_window)
	{
		Gtk::Allocation allocation = get_allocation();
		Cairo::RefPtr<Cairo::Context> cr = ref_window->create_cairo_context();
		cr->rectangle(event->area.x, event->area.y, event->area.width, event->area.height);
		cr->clip();

		Pango::FontDescription font_desc("sans 12");
		
		//draw vert labels
		bool is_intersect = false;
		for(size_t i = 0; i < m_ColumnsCount; ++ i)
		{
			Gdk::Rectangle rect = get_label_vert_rect(i);
			expose_rect.intersect(rect, is_intersect);
			if(is_intersect)
			{
				Glib::RefPtr<Pango::Layout> layout = Pango::Layout::create(cr);
				Glib::RefPtr<Pango::Context> context = layout->get_context();
				layout->set_font_description(font_desc);
				layout->set_text(m_VertLabels[i]);
				ref_window->draw_layout(Gdk::GC::create(ref_window), rect.get_x(), rect.get_y(), layout);
			}
		}

		//draw horz labels
		for(size_t j = 0; j < m_RowsCount; ++ j)
		{
			Gdk::Rectangle rect = get_label_horz_rect(j);
			expose_rect.intersect(rect, is_intersect);
			if(is_intersect)
			{
				Glib::RefPtr<Pango::Layout> layout = Pango::Layout::create(cr);
				Glib::RefPtr<Pango::Context> context = layout->get_context();
				layout->set_font_description(font_desc);
				layout->set_text(m_HorzLabels[j]);
				ref_window->draw_layout(Gdk::GC::create(ref_window), rect.get_x(), rect.get_y(), layout);
			}
		}

		//draw cells labels
		for(size_t i = 0; i < m_RowsCount; ++ i)
		{
			for(size_t j = 0; j < m_ColumnsCount; ++ j)
			{
				Gdk::Rectangle rect = get_cell_rect(i, j);
				expose_rect.intersect(rect, is_intersect);
				if(is_intersect)
				{
					Glib::RefPtr<Pango::Layout> layout = Pango::Layout::create(cr);
					Glib::RefPtr<Pango::Context> context = layout->get_context();
					layout->set_font_description(font_desc);
					layout->set_text(m_TextMatrix[i][j]);
					ref_window->draw_layout(Gdk::GC::create(ref_window), rect.get_x(), rect.get_y(), layout);
				}
			}
		}


	}
	return true;
}

void Sheet::on_size_request(Gtk::Requisition* requisition)
{
	requisition->height = m_HorzLabelHeight + m_RowsCount * m_CellHeight;
	requisition->width = m_VertLabelWidth + m_ColumnsCount * m_CellWidth;
}

Sheet::~Sheet()
{
}

void Sheet::set_rows_count(size_t rows)
{
	m_RowsCount = rows;
	m_VertLabels.resize(rows);
	m_TextMatrix.resize(rows, std::vector<Glib::ustring>(m_ColumnsCount));
}

void Sheet::set_columns_count(size_t columns)
{
	m_ColumnsCount = columns;
	m_HorzLabels.resize(columns);
	for(std::vector<std::vector<Glib::ustring> >::iterator it = m_TextMatrix.begin(); it != m_TextMatrix.end(); ++ it)
	{
		it->resize(columns);
	}
}

