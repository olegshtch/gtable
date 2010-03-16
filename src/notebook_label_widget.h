#ifndef _NOTEBOOK_LABEL_WIDGET_H_
#define _NOTEBOOK_LABEL_WIDGET_H_

#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/button.h>

class NotebookLabelWidget : public Gtk::HBox
{
public:
	NotebookLabelWidget(const Glib::ustring &label);
	~NotebookLabelWidget();
private:
	Gtk::Label m_Label;
	Gtk::Image m_Img;
	Gtk::Button m_Button;

	void OnClick();
};

#endif

