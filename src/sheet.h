#ifndef _SHEET_H_
#define _SHEET_H_

#include <gtkmm/treeview.h>
#include <gtkmm/cellrenderertext.h>
#include <gtkmm/builder.h>
#include <vector>
#include "orm/data.h"
#include "id_text_scheme.h"

class Sheet : public Gtk::TreeView
{
public:
	Sheet();
	Sheet(GtkTreeView *cobject, const Glib::RefPtr<Gtk::Builder>& builder);
	~Sheet();

	void set_horz_model(const Glib::RefPtr<ORM::Data>& horz_model);
	void set_vert_model(const Glib::RefPtr<ORM::Data>& vert_model);

	typedef sigc::signal<void, Gtk::CellRenderer*, long int, long int> signal_cell_data_t;
	signal_cell_data_t& signal_cell_data()
	{
		return signal_cell_data_;
	}

	typedef sigc::signal<void, long int, long int, GdkEventButton*> signal_cell_button_release_t;
	signal_cell_button_release_t& signal_cell_button_release()
	{
		return signal_cell_button_release_;
	}
protected:
	void on_label_data(Gtk::CellRenderer* cell, const Gtk::TreeModel::iterator& iter);
	void on_cell_data(Gtk::CellRenderer* cell, const Gtk::TreeModel::iterator& iter, long int column_id);

	bool on_button_release_event(GdkEventButton* event);

	signal_cell_data_t signal_cell_data_;
	signal_cell_button_release_t signal_cell_button_release_;
private:
	void Init();
	void set_columns_count(size_t columns);

	IdTextScheme m_ColumnRecord;
	Glib::RefPtr<ORM::Data> m_refVertModel;
	Glib::RefPtr<ORM::Data> m_refHorzModel;
	Gtk::CellRendererText m_CellRenderer;
	Gtk::CellRendererText m_LabelRenderer;
	size_t m_RowsCount;
	size_t m_ColumnsCount;
};

#endif

