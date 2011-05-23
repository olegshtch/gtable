#include <gtkmm/stock.h>
#include <gtkmm/filechooserdialog.h>
#include <iostream>
#include <vector>
#include "export_dialog.h"
#include "db/id_text_scheme.h"
#include "db/models.h"
#include "db/db.h"
#include "shared.h"
#include "3rd_party/BasicExcel.hpp"
#include "3rd_party/ExcelFormat.h"

#ifdef WIN32
#include "../win32/win32.h"
#endif

ExportDialog::ExportDialog(GtkDialog *cobject, const Glib::RefPtr<Gtk::Builder>& builder)
	:Glib::ObjectBase(typeid(ExportDialog))
	,Gtk::Dialog(cobject)
	,m_ShowAuditorium(NULL)
	,m_ExportCategory(NULL)
	,m_ExportedObjects(NULL)
	,m_LabelExportCategory(NULL)
	,m_refCategory(ORM::Data::create(DB::g_IdTextScheme))
	,m_refChooseModel(Gtk::ListStore::create(m_ChooseModel))
{
	add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_YES);

	builder->get_widget("CheckShowAuditorium", m_ShowAuditorium);
	if(! m_ShowAuditorium)
	{
		throw Glib::Error(1, 0, "Cann't load CheckShowAuditorium.");
	}
	builder->get_widget("ExportCategory", m_ExportCategory);
	if(! m_ExportCategory)
	{
		throw Glib::Error(1, 0, "Cann't load ExportCategory.");
	}
	builder->get_widget("TreeViewExportedObjects", m_ExportedObjects);
	if(! m_ExportedObjects)
	{
		throw Glib::Error(1, 0, "Cann't load TreeViewExportedObjects.");
	}
	builder->get_widget("LabelObjectCategory", m_LabelExportCategory);
	if(! m_LabelExportCategory)
	{
		throw Glib::Error(1, 0, "Cann't load LabelObjectCategory.");
	}

	m_ExportCategory->set_model(m_refCategory);
	m_ExportCategory->pack_start(DB::g_IdTextScheme.fText);
	Gtk::TreeIter iter = m_refCategory->append();
	iter->set_value(DB::g_IdTextScheme.fId, 1l);
	iter->set_value(DB::g_IdTextScheme.fText, Glib::ustring(_("Группы")));
	m_ExportCategory->set_active(iter);
	/*iter = m_refCategory->append();
	iter->set_value(DB::g_IdTextScheme.fId, 2l);
	iter->set_value(DB::g_IdTextScheme.fText, Glib::ustring(_("Teachers")));
	iter = m_refCategory->append();
	iter->set_value(DB::g_IdTextScheme.fId, 3l);
	iter->set_value(DB::g_IdTextScheme.fText, Glib::ustring(_("Auditoriums")));*/
	m_ExportCategory->signal_changed().connect(sigc::mem_fun(*this, &ExportDialog::OnExportChanged));

	m_ExportedObjects->set_headers_visible(false);
	m_ExportedObjects->set_model(m_refChooseModel);
	m_ExportedObjects->append_column_editable("", m_ChooseModel.fChoose);
	m_ExportedObjects->append_column("", m_ChooseModel.fText);

	//load groups
	
	Glib::RefPtr<ORM::Data> groups = ORM::Data::create(DB::g_IdTextScheme);
	DB::DataBase::Instance().ListEntitiesTextOrdered(DB::g_ModelGroups, DB::g_ModelGroups.name, groups);
	for(Gtk::TreeIter iter = groups->children().begin(); iter != groups->children().end(); ++ iter)
	{
		Gtk::TreeIter add_iter = m_refChooseModel->append();
		add_iter->set_value(m_ChooseModel.fId, iter->get_value(DB::g_IdTextScheme.fId));
		add_iter->set_value(m_ChooseModel.fText, iter->get_value(DB::g_IdTextScheme.fText));
	}
	
	show_all_children();
}

void ExportDialog::OnExportChanged()
{
}

void ExportDialog::on_response(int response_id)
{
	if(response_id == Gtk::RESPONSE_YES)
	{
#if WIN32
		Win32FileDialog dialog(_("Выбрать файл для экспорта расписания:"),
			Gtk::FILE_CHOOSER_ACTION_SAVE);
		dialog.add_filter(_("Microsoft Excel"), "*.xls");
		dialog.add_filter(_("Все файлы"), "*");
		dialog.set_def_ext("xls");
#else
		Gtk::FileChooserDialog dialog(*this, _("Choose file for exporting timetable:"),
		Gtk::FILE_CHOOSER_ACTION_SAVE);
		dialog.add_button(Gtk::Stock::CANCEL,Gtk::RESPONSE_CANCEL);
		dialog.add_button(Gtk::Stock::SAVE,Gtk::RESPONSE_YES);
		Gtk::FileFilter filter_tbl;
		filter_tbl.set_name(_("Microsoft Excel"));
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
			Export(filename);
		}
	}
	hide();
}

void ExportDialog::Export(const Glib::ustring& filename)
{
	size_t merged_lines = m_ShowAuditorium->get_active() ? 3 : 2;
	YExcel::BasicExcel xls;
	xls.New(1);
	YExcel::BasicExcelWorksheet *worksheet = xls.GetWorksheet(0);
	if(! worksheet)
	{
		throw Glib::Error(1, 0, "Cann't get worksheet");
	}
	worksheet->Rename(WChar(_("Расписание")));
	ExcelFormat::XLSFormatManager format_manager(xls);
	ExcelFormat::CellFormat cell_format(format_manager);
	cell_format.set_alignment(ExcelFormat::EXCEL_VALIGN_CENTRED | ExcelFormat::EXCEL_HALIGN_CENTRED);
	cell_format.set_wrapping(false);
	cell_format.set_borderlines(ExcelFormat::EXCEL_LS_THICK
		, ExcelFormat::EXCEL_LS_THICK
		, ExcelFormat::EXCEL_LS_THICK
		, ExcelFormat::EXCEL_LS_THICK
		, 0
		, 0);
	YExcel::BasicExcelCell *cell = worksheet->Cell(1, 0);
	cell->Set(WChar(_("День")));
	cell->SetFormat(cell_format);
	worksheet->SetColWidth(0, 5000);
	cell = worksheet->Cell(1, 1);
	cell->Set(WChar(_("Час")));
	cell->SetFormat(cell_format);
	worksheet->SetColWidth(1, 5000);
	cell = worksheet->Cell(0, 2);
	cell->Set(WChar(_("Группы")));
	cell->SetFormat(cell_format);

	//заполнить группами
	std::vector<std::pair<long int, Glib::ustring> > groups_list;
	Gtk::TreeIter iter;
	for(iter = m_refChooseModel->children().begin(); iter != m_refChooseModel->children().end(); ++ iter)
	{
		if(iter->get_value(m_ChooseModel.fChoose))
		{
			groups_list.push_back(std::make_pair(iter->get_value(m_ChooseModel.fId), iter->get_value(m_ChooseModel.fText)));
		}
	}

	cell->SetMergedColumns(groups_list.size());
	for(size_t i = 0; i < groups_list.size(); ++ i)
	{
		cell = worksheet->Cell(1, i + 2);
		cell->Set(WChar(groups_list[i].second));
		cell->SetFormat(cell_format);
		cell = worksheet->Cell(0, i + 2);
		cell->SetFormat(cell_format);
		worksheet->SetColWidth(i + 2, 5000);
	}

	Glib::RefPtr<ORM::Data> days_list = ORM::Data::create(DB::g_IdTextScheme);
	Glib::RefPtr<ORM::Data> hours_list = ORM::Data::create(DB::g_IdTextScheme);
	DB::DataBase::Instance().ListEntitiesTextOrderedID(DB::g_ModelDays, DB::g_ModelDays.name, days_list);
	DB::DataBase::Instance().ListEntitiesTextOrderedID(DB::g_ModelHours, ORM::Expr<Glib::ustring>(ORM::Expr<Glib::ustring>(DB::g_ModelHours.start) + "-" + DB::g_ModelHours.finish), hours_list);

	ExcelFormat::CellFormat cell_top(format_manager);
	cell_top.set_borderlines(ExcelFormat::EXCEL_LS_THICK
		, ExcelFormat::EXCEL_LS_THICK
		, ExcelFormat::EXCEL_LS_THICK
		, ExcelFormat::EXCEL_LS_NO_LINE
		, 0
		, 0);

	ExcelFormat::CellFormat cell_bottom(format_manager);
	cell_bottom.set_borderlines(ExcelFormat::EXCEL_LS_THICK
		, ExcelFormat::EXCEL_LS_THICK
		, ExcelFormat::EXCEL_LS_NO_LINE
		, ExcelFormat::EXCEL_LS_THICK
		, 0
		, 0);

	ExcelFormat::CellFormat cell_middle(format_manager);
	cell_middle.set_borderlines(ExcelFormat::EXCEL_LS_THICK
		, ExcelFormat::EXCEL_LS_THICK
		, ExcelFormat::EXCEL_LS_NO_LINE
		, ExcelFormat::EXCEL_LS_NO_LINE
		, 0
		, 0);

	size_t row = 2;
	for(Gtk::TreeIter day = days_list->children().begin(); day != days_list->children().end(); ++ day)
	{
		cell = worksheet->Cell(row, 0);
		cell->SetMergedRows(hours_list->children().size() * merged_lines);
		cell->Set(WChar(day->get_value(DB::g_IdTextScheme.fText)));
		cell->SetFormat(cell_format);
		for(Gtk::TreeIter hour = hours_list->children().begin(); hour != hours_list->children().end(); ++ hour, row += merged_lines)
		{
			cell = worksheet->Cell(row, 1);
			cell->SetMergedRows(merged_lines);
			cell->Set(WChar(hour->get_value(DB::g_IdTextScheme.fText)));
			cell->SetFormat(cell_format);
			for(size_t i = 0; i < groups_list.size(); ++ i)
			{
				Glib::ustring timetablestr = DB::DataBase::Instance().GetTimeTableLessonGroupText(groups_list[i].first, hour->get_value(DB::g_IdTextScheme.fId), day->get_value(DB::g_IdTextScheme.fId), m_ShowAuditorium->get_active());
				Glib::ustring::size_type index_start = 0;
				for(size_t j = 0; j < merged_lines; ++ j)
				{
					cell = worksheet->Cell(row + j, 0);
					cell->SetFormat(cell_format);
					cell = worksheet->Cell(row + j, 1);
					cell->SetFormat(cell_format);

					cell = worksheet->Cell(row + j, i + 2);
					cell->SetFormat(cell_middle);
					Glib::ustring::size_type index_end = timetablestr.find('\n', index_start);
					cell->SetWString(WChar(timetablestr.substr(index_start, index_end)));
					index_start = index_end + 1;
					if(j == 0)
					{
						cell->SetFormat(cell_top);
					}
					else if(j == (merged_lines - 1))
					{
						cell->SetFormat(cell_bottom);
					}
				}
			}
		}
	}

	xls.SaveAs(WChar(filename));
}

