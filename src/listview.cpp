#include <iostream>
#include "cellrendererforeign.h"
#include "listview.h"

/*void ListView::on_row_changed(const Gtk::TreeModel::Path& path, const Gtk::TreeModel::iterator& iter)
{
	if(m_Refresh)
	{
		std::cout << "update row " << path.to_string() << std::endl;
		DB::DataBase::Instance().EditEntity(*m_Scheme, m_refModel->get_iter(path));
	}
	signal_list_edited_.emit();
}*/

int ListView::append_column_foreign_editable(const Glib::ustring& str, const ORM::Field<ORM::ForeignKey>& field, const ORM::Table& foreign_table, const ORM::Field<Glib::ustring>& foreign_field)
{
	Gtk::TreeViewColumn *p_column = Gtk::manage(new Gtk::TreeViewColumn(str));
	CellRendererForeign *p_renderer = Gtk::manage(new CellRendererForeign(field, foreign_table, foreign_field));
	p_column->pack_start(*p_renderer);
	p_renderer->signal_edited().connect(sigc::bind(sigc::mem_fun(*this, &ListView::OnForeignEdited), &field));

	Gtk::TreeViewColumn::SlotCellData slot = sigc::ptr_fun(CellRendererForeign::ForeignKeyAsString);

	p_column->set_cell_data_func(*p_renderer, slot);

	return append_column(*p_column);
}

int ListView::append_column_text_editable(const Glib::ustring& str, const ORM::Field<Glib::ustring>& field)
{
	Gtk::TreeViewColumn *p_column = Gtk::manage(new Gtk::TreeViewColumn(str, field));
	Gtk::CellRendererText *p_renderer = dynamic_cast<Gtk::CellRendererText *>(p_column->get_first_cell_renderer());
	if(p_renderer)
	{
		p_renderer->property_editable() = true;
		p_renderer->signal_edited().connect(sigc::bind(sigc::mem_fun(*this, &ListView::OnTextEdited), &field));
		return append_column(*p_column);
	}
	else
	{
		delete p_column;
		return -1;
	}
}

int ListView::append_column_bool_editable(const Glib::ustring& str, const ORM::Field<bool>& field)
{
	Gtk::TreeViewColumn *p_column = Gtk::manage(new Gtk::TreeViewColumn(str, field));
	Gtk::CellRendererToggle *p_renderer = dynamic_cast<Gtk::CellRendererToggle *>(p_column->get_first_cell_renderer());
	if(p_renderer)
	{
		p_renderer->property_activatable() = true;
		p_renderer->signal_toggled().connect(sigc::bind(sigc::mem_fun(*this, &ListView::OnBoolEdited), &field));
		return append_column(*p_column);
	}
	else
	{
		delete p_column;
		return -1;
	}
}

int ListView::append_column_int_editable(const Glib::ustring& str, const ORM::Field<long int>& field)
{
	Gtk::TreeViewColumn *p_column = Gtk::manage(new Gtk::TreeViewColumn(str, field));
	Gtk::CellRendererText *p_renderer = dynamic_cast<Gtk::CellRendererText *>(p_column->get_first_cell_renderer());
	if(p_renderer)
	{
		p_renderer->property_editable() = true;
		p_renderer->signal_edited().connect(sigc::bind(sigc::mem_fun(*this, &ListView::OnIntEdited), &field));
		return append_column(*p_column);
	}
	else
	{
		delete p_column;
		return -1;
	}
}

void ListView::OnForeignEdited(const Glib::ustring& path, long int id, const ORM::Field<ORM::ForeignKey> *field)
{
	DB::DataBase::Instance().EditEntityField(*m_Scheme, m_refModel->get_iter(path)->get_value(m_Scheme->fId), *field, ORM::ForeignKey(id));
	m_refModel->get_iter(path)->set_value(*field, id);
	signal_list_edited_.emit();
}

void ListView::OnTextEdited(const Glib::ustring& path, const Glib::ustring& new_text, const ORM::Field<Glib::ustring> *field)
{
	DB::DataBase::Instance().EditEntityField(*m_Scheme, m_refModel->get_iter(path)->get_value(m_Scheme->fId), *field, new_text);
	m_refModel->get_iter(path)->set_value(*field, new_text);
	signal_list_edited_.emit();
}

void ListView::OnBoolEdited(const Glib::ustring& path, const ORM::Field<bool> *field)
{
	Gtk::TreeIter iter = m_refModel->get_iter(path);
	if(iter)
	{
		bool value = ! iter->get_value(*field);
		iter->set_value(*field, value);
		DB::DataBase::Instance().EditEntityField(*m_Scheme, iter->get_value(m_Scheme->fId), *field, value);
	}
}

void ListView::OnIntEdited(const Glib::ustring& path, const Glib::ustring& new_text, const ORM::Field<long int> *field)
{
	Gtk::TreeIter iter = m_refModel->get_iter(path);
	if(iter)
	{
		long int value;
		long int id = iter->get_value(m_Scheme->fId);
		std::stringstream stream;
		stream << new_text;
		stream >> value;
		DB::DataBase::Instance().EditEntityField(*m_Scheme, id, *field, value);
		iter->set_value(*field, value);
	}
}

void ListView::SelectionChanged()
{
	Gtk::TreeIter iter = get_selection()->get_selected();
	if(iter)
	{
		signal_choose_object_.emit(iter->get_value(m_Scheme->fId));
	}
}

