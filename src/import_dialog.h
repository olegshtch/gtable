#ifndef _IMPORT_DIALOG_H_
#define _IMPORT_DIALOG_H_

#include <gtkmm/dialog.h>
#include <gtkmm/combobox.h>
#include <gtkmm/comboboxentry.h>
#include "sheet.h"
#include "3rd_party/BasicExcel.hpp"
#include "3rd_party/ExcelFormat.h"

class ImportDialog : public Gtk::Dialog
{
public:
	ImportDialog(GtkDialog *cobject, const Glib::RefPtr<Gtk::Builder>& builder);
	virtual ~ImportDialog()
	{
	}
private:
	virtual void on_response(int response_id);

	void OnWorksheet();

	void OnCellData(Gtk::CellRenderer* cell, long int row, long int col);

	Gtk::ComboBoxEntry *m_Speciality;
	Gtk::ComboBox *m_WorkSheet;
	Sheet *m_TeachPlan;
	Glib::RefPtr<ORM::Data> m_refBranches;
	Glib::RefPtr<ORM::Data> m_refTypes;

	YExcel::BasicExcel m_Xls;
	YExcel::BasicExcelWorksheet *m_pWorkSheet;
};

#endif

