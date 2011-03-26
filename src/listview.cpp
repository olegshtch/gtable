#include <iostream>
#include "listview.h"

void ListView::on_row_changed(const Gtk::TreeModel::Path& path, const Gtk::TreeModel::iterator& iter)
{
	std::cout << "update row " << path.to_string() << std::endl;
	DB::DataBase::Instance().EditEntity(*m_Scheme, iter);
}

