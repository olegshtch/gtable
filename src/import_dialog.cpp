#include <gtkmm/stock.h>
#include <gtkmm/filechooserdialog.h>
#include <iostream>
#include "import_dialog.h"
#include "shared.h"
#include "db/db.h"
#include "db/id_text_scheme.h"

#ifdef WIN32
#include "../win32/win32.h"
#endif

ImportDialog::ImportDialog(GtkDialog *cobject, const Glib::RefPtr<Gtk::Builder>& builder)
	:Glib::ObjectBase(typeid(ImportDialog))
	,Gtk::Dialog(cobject)
	,m_Speciality(NULL)
	,m_WorkSheet(NULL)
	,m_TeachPlan(NULL)
	,m_refBranches(ORM::Data::create(DB::g_IdTextScheme))
	,m_refTypes(ORM::Data::create(DB::g_IdTextScheme))
	,m_pWorkSheet(NULL)
{
#if WIN32
	Win32FileDialog dialog(_("Choose file for import:"),
		Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.add_filter(_("MS Excel"), "*.xls");
	dialog.add_filter(_("All files"), "*");
	dialog.set_def_ext("xls");
#else
	Gtk::FileChooserDialog dialog(*this,_("Choose file for import:"),
		Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.add_button(Gtk::Stock::CANCEL,Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::OPEN,Gtk::RESPONSE_YES);
	Gtk::FileFilter filter_tbl;
	filter_tbl.set_name(_("MS Excel"));
	filter_tbl.add_pattern("*.xls");
	dialog.add_filter(filter_tbl);
	Gtk::FileFilter filter_all;
	filter_all.set_name(_("All files"));
	filter_all.add_pattern("*");
	dialog.add_filter(filter_all);
#endif
	if(dialog.run()==Gtk::RESPONSE_YES)
	{
		Glib::ustring filename = dialog.get_filename();
		if(filename.substr(filename.length() - 4, 4) != ".xls")
		{
			filename += ".xls";
		}
		add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_YES);

		builder->get_widget("ImportSpeciality", m_Speciality);
		if(! m_Speciality)
		{
			throw Glib::Error(1, 0, "Cann't load ImportSpeciality");
		}
		builder->get_widget("ImportWorkSheet", m_WorkSheet);
		if(! m_WorkSheet)
		{
			throw Glib::Error(1, 0, "Cann't load ImportWorkSheet");
		}
		builder->get_widget_derived("ImportTeachingPlan", m_TeachPlan);
		if(! m_TeachPlan)
		{
			throw Glib::Error(1, 0, "Cann't load ImportTeachingPlan");
		}
		
		Glib::RefPtr<ORM::Data> specialities = ORM::Data::create(DB::g_IdTextScheme);
		DB::DataBase::Instance().ListEntitiesTextOrdered(DB::g_ModelSpecialities, DB::g_ModelSpecialities.name, specialities);
		m_Speciality->set_model(specialities);
		m_Speciality->set_text_column(DB::g_IdTextScheme.fText);

		Glib::RefPtr<ORM::Data> worksheets = ORM::Data::create(DB::g_IdTextScheme);
		m_Xls.Load(WChar(filename));
		int count = m_Xls.GetTotalWorkSheets();
		for(size_t i = 0; i < count; ++ i)
		{
			Gtk::TreeIter iter = worksheets->append();
			iter->set_value(DB::g_IdTextScheme.fText, w2ustring(m_Xls.GetUnicodeSheetName(i)));
			iter->set_value(DB::g_IdTextScheme.fId, static_cast<long>(i));
		}
		m_WorkSheet->set_model(worksheets);
		m_WorkSheet->pack_start(DB::g_IdTextScheme.fText);
		m_WorkSheet->signal_changed().connect(sigc::mem_fun(*this, &ImportDialog::OnWorksheet));

		m_TeachPlan->set_vert_model(m_refBranches);
		m_TeachPlan->set_horz_model(m_refTypes);
		m_TeachPlan->signal_cell_data().connect(sigc::mem_fun(*this, &ImportDialog::OnCellData));

		show_all_children();
	}
	else
	{
		hide();
	}
}

void ImportDialog::on_response(int response_id)
{
	if(response_id == Gtk::RESPONSE_YES)
	{
		//add specialtiy
		DB::DataBase &db = DB::DataBase::Instance();
		long int id_speciality = db.AppendEntityByName(DB::g_ModelSpecialities, m_Speciality->get_active_text());

		for(Gtk::TreeIter branch = m_refBranches->children().begin(); branch != m_refBranches->children().end(); ++ branch)
		{
			long int id_branch = db.AppendEntityByName(DB::g_ModelBranch, branch->get_value(DB::g_IdTextScheme.fText));
			long int id_teaching_branch = db.AppendNewBranchForSpeciality(id_speciality, id_branch);
			for(Gtk::TreeIter type = m_refTypes->children().begin(); type != m_refTypes->children().end(); ++ type)
			{
				long int id_type = db.AppendEntityByName(DB::g_ModelLessonType, type->get_value(DB::g_IdTextScheme.fText));
				long hours = 0;

				YExcel::BasicExcelCell *cell = m_pWorkSheet->Cell(branch->get_value(DB::g_IdTextScheme.fId), type->get_value(DB::g_IdTextScheme.fId));
				switch(cell->Type())
				{
				case YExcel::BasicExcelCell::INT:
					hours = cell->GetInteger();
					break;
				case YExcel::BasicExcelCell::DOUBLE:
					hours = cell->GetDouble();
					break;
				}
				db.EditTeachingPlanHours(id_teaching_branch, id_type, hours);
			}
		}
	}
	hide();
}

void ImportDialog::OnWorksheet()
{
	Gtk::TreeIter iter = m_WorkSheet->get_active();
	if(iter)
	{
		m_pWorkSheet = m_Xls.GetWorksheet(iter->get_value(DB::g_IdTextScheme.fId));
		m_refBranches->clear();
		size_t row = 6;
		while(true)
		{
			YExcel::BasicExcelCell *cell = m_pWorkSheet->Cell(row, 1);
			const wchar_t* name = cell->GetWString();
			if(name && (name[0] != L'\0'))
			{
				Gtk::TreeIter branch = m_refBranches->append();
				branch->set_value(DB::g_IdTextScheme.fText, w2ustring(name));
				branch->set_value(DB::g_IdTextScheme.fId, static_cast<long int>(row));
			}
			else
			{
				break;
			}
			++ row;
		}
		//

		m_refTypes->clear();
		size_t col = 4;
		while(true)
		{
			YExcel::BasicExcelCell *cell1 = m_pWorkSheet->Cell(4, col);
			YExcel::BasicExcelCell *cell2 = m_pWorkSheet->Cell(5, col);
			const wchar_t* name1 = cell1->GetWString();
			const wchar_t* name2 = cell2->GetWString();
			Glib::ustring name;
			if(name1 && (name1[0] != L'\0'))
			{
				name += w2ustring(name1);
			}
			if(name2 && (name2[0] != L'\0'))
			{
				name += w2ustring(name2);
			}

			if(name.empty())
			{
				break;
			}
			else
			{
				Gtk::TreeIter typeiter = m_refTypes->append();
				typeiter->set_value(DB::g_IdTextScheme.fText, name);
				typeiter->set_value(DB::g_IdTextScheme.fId, static_cast<long int>(col));
			}
			++ col;
		}
		m_TeachPlan->set_horz_model(m_refTypes);
	}
}

void ImportDialog::OnCellData(Gtk::CellRenderer* cell, long int row, long int col)
{
	Gtk::CellRendererText *renderer = reinterpret_cast<Gtk::CellRendererText *>(cell);
	long int hours = 0;
	if(m_pWorkSheet)
	{
		YExcel::BasicExcelCell *xcell = m_pWorkSheet->Cell(row, col);
		
		switch(xcell->Type())
		{
		case YExcel::BasicExcelCell::INT:
			renderer->property_background() = "red";
			hours = xcell->GetInteger();
			break;
		case YExcel::BasicExcelCell::DOUBLE:
			renderer->property_background() = "green";
			hours = xcell->GetDouble();
			break;
		default:
			renderer->property_background() = "white";
		}
	}
	if(hours)
	{
		renderer->property_text() = ORM::Field<long int>::ToString(hours);
	}
	else
	{
		renderer->property_text() = Glib::ustring();
	}
}

