#include <gtkmm/stock.h>
#include <gtkmm/notebook.h>
#include <iostream>
#include "notebook_label_widget.h"

NotebookLabelWidget::NotebookLabelWidget(const Glib::ustring &label)
	:m_Label(label), m_Img(Gtk::Stock::CLOSE, Gtk::ICON_SIZE_SMALL_TOOLBAR) //, m_Button(Gtk::Stock::CLOSE)
{
	//m_Button.set_label("");
	m_Button.set_image(m_Img);
	m_Button.set_relief(Gtk::RELIEF_NONE);
	pack_start(m_Label, Gtk::PACK_SHRINK);
	pack_end(m_Button, Gtk::PACK_SHRINK);

	m_Button.signal_clicked().connect(sigc::mem_fun(*this, &NotebookLabelWidget::OnClick));

	show_all_children();
}

NotebookLabelWidget::~NotebookLabelWidget()
{
}

void NotebookLabelWidget::OnClick()
{
//	std::cout << typeid(*get_parent()).name() << std::endl;
	Gtk::Notebook &notebook = *dynamic_cast<Gtk::Notebook*>(get_parent());
	for(Gtk::Notebook_Helpers::PageIterator it = notebook.pages().begin(); it != notebook.pages().end(); it ++)
	{
		if(it->get_tab_label() == this)
		{
			notebook.remove_page(*it->get_child());
			return;
		}
	}
}

