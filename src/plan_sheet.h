#ifndef _PLAN_SHEET_
#define _PLAN_SHEET_

//#include <gtkmm/cellrenderercombo.h>
#include "sheet.h"
#include "line_editable.h"
#include "cellrendererforeign.h"

class PlanSheet : public Sheet, public LineEditable
{
public:
	PlanSheet(GtkTreeView *cobject, const Glib::RefPtr<Gtk::Builder>& builder);
	~PlanSheet();

	void add_empty_line();
	void remove_line();
	void update_model();

	void set_speciality(long int id_speciality);
private:
	void BranchEdited(const Glib::ustring& path, long int id);
	void on_label_data(Gtk::CellRenderer* cell, const Gtk::TreeModel::iterator& iter);
	void on_label_button_release(const Gtk::TreeIter& iter, GdkEventButton* event);

	CellRendererForeign m_BranchRenderer;

	long int m_IdSpeciality;
};

#endif
