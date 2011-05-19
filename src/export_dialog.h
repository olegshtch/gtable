#ifndef _EXPORT_DIALOG_H_
#define _EXPORT_DIALOG_H_

#include <gtkmm/dialog.h>
#include <gtkmm/builder.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/box.h>
#include <gtkmm/combobox.h>
#include <gtkmm/treeview.h>
#include <gtkmm/label.h>
#include "orm/data.h"

class ExportDialog : public Gtk::Dialog
{
public:
	ExportDialog(GtkDialog *cobject, const Glib::RefPtr<Gtk::Builder>& builder);
	virtual ~ExportDialog()
	{
	}
private:
	class ChooseModel : public Gtk::TreeModelColumnRecord
	{
	public:
		Gtk::TreeModelColumn<bool> fChoose;
		Gtk::TreeModelColumn<long int> fId;
		Gtk::TreeModelColumn<Glib::ustring> fText;

		ChooseModel()
		{
			add(fChoose);
			add(fId);
			add(fText);
		}
	};

	void OnExportChanged();

	virtual void on_response(int response_id);

	void Export(const Glib::ustring& filename);

	Gtk::CheckButton *m_ShowAuditorium;
	Gtk::ComboBox *m_ExportCategory;
	Gtk::TreeView *m_ExportedObjects;
	Gtk::Label *m_LabelExportCategory;

	Glib::RefPtr<ORM::Data> m_refCategory;
	ChooseModel m_ChooseModel;
	Glib::RefPtr<Gtk::ListStore> m_refChooseModel;
};

#endif

