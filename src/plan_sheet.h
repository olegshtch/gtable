#ifndef _PLAN_SHEET_
#define _PLAN_SHEET_

#include "sheet.h"
#include "line_editable.h"

class PlanSheet : public Sheet, public LineEditable
{
public:
	PlanSheet();
	PlanSheet(GtkTreeView *cobject, const Glib::RefPtr<Gtk::Builder>& builder);
	~PlanSheet();

	void add_empty_line();
	void remove_line();
	void update_model();
private:
};

#endif