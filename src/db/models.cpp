#include <gtkmm/liststore.h>
#include <iostream>
#include "models.h"

const DB::ModelEntity DB::g_ModelEntity;
const DB::ModelAud DB::g_ModelAud;
const DB::ModelOrder DB::g_ModelOrder;
const DB::ModelPlan DB::g_ModelPlan;
const DB::ModelLessonTeacher DB::g_ModelLessonTeacher;

int DB::ModelEntity::Callback(void *list_ptr, int argc, char **argv, char **col_name)
{
	if(! list_ptr)
	{
		throw Glib::Error(1, 0, "Null this at ModelEntity::Callback.");
	}
	if(argc != 2)
	{
		throw Glib::Error(1, 0, "Not 2 columns at ModelEntity::Callback.");
	}

	Gtk::ListStore *list_store = reinterpret_cast<Gtk::ListStore*>(list_ptr);
	Gtk::TreeIter it = list_store->append();
	std::clog << " [" << col_name[0] << "]=" << argv[0];
	it->set_value(DB::g_ModelEntity.id, atol(argv[0]));
	std::clog << " [" << col_name[1] << "]=" << argv[1];
	it->set_value(DB::g_ModelEntity.name, Glib::ustring(argv[1]));
	std::clog << std::endl;
	return 0;
}

int DB::ModelAud::Callback(void *list_ptr, int argc, char **argv, char **col_name)
{
	if(! list_ptr)
	{
		throw Glib::Error(1, 0, "Null this at ModelAud::Callback.");
	}
	if(argc != 3)
	{
		throw Glib::Error(1, 0, "Not 3 columns at ModelAud::Callback.");
	}

	Gtk::ListStore *list_store = reinterpret_cast<Gtk::ListStore*>(list_ptr);
	Gtk::TreeIter it = list_store->append();
	std::clog << " [" << col_name[0] << "]=" << argv[0];
	it->set_value(DB::g_ModelAud.id, atol(argv[0]));
	std::clog << " [" << col_name[1] << "]=" << argv[1];
	it->set_value(DB::g_ModelAud.name, Glib::ustring(argv[1]));
	std::clog << " [" << col_name[2] << "]=" << argv[2];
	it->set_value(DB::g_ModelAud.multithread, static_cast<bool>(atoi(argv[2])));
	std::clog << std::endl;
	return 0;
}

int DB::ModelOrder::Callback(void *list_ptr, int argc, char **argv, char **col_name)
{
	if(! list_ptr)
	{
		throw Glib::Error(1, 0, "Null this at ModelOrder::Callback.");
	}
	if(argc != 4)
	{
		throw Glib::Error(1, 0, "Not 4 columns at ModelOrder::Callback.");
	}

	Gtk::ListStore *list_store = reinterpret_cast<Gtk::ListStore*>(list_ptr);
	Gtk::TreeIter it = list_store->append();
	std::clog << " [" << col_name[0] << "]=" << argv[0];
	it->set_value(DB::g_ModelOrder.id, atol(argv[0]));
	std::clog << " [" << col_name[1] << "]=" << argv[1];
	it->set_value(DB::g_ModelOrder.name, Glib::ustring(argv[1]));
	std::clog << " [" << col_name[2] << "]=" << argv[2];
	it->set_value(DB::g_ModelOrder.l_id, atol(argv[2]));
	std::clog << " [" << col_name[3] << "]=" << argv[3];
	it->set_value(DB::g_ModelOrder.l_name, Glib::ustring(argv[3]));
	std::clog << std::endl;
	return 0;
}

int DB::ModelLessonTeacher::Callback(void *list_ptr, int argc, char **argv, char **col_name)
{
	if(! list_ptr)
	{
		throw Glib::Error(1, 0, "Null this at ModelLessonTeacher::Callback.");
	}
	if(argc != 5)
	{
		throw Glib::Error(1, 0, "Not 5 columns at ModelLessonTeacher::Callback.");
	}

	Gtk::ListStore *list_store = reinterpret_cast<Gtk::ListStore*>(list_ptr);
	Gtk::TreeIter it = list_store->append();
	std::clog << " [" << col_name[0] << "]=" << argv[0];
	it->set_value(DB::g_ModelLessonTeacher.id, atol(argv[0]));
	std::clog << " [" << col_name[1] << "]=" << argv[1];
	it->set_value(DB::g_ModelLessonTeacher.name, Glib::ustring(argv[1]));
	std::clog << " [" << col_name[2] << "]=" << argv[2];
	it->set_value(DB::g_ModelLessonTeacher.l_id, atol(argv[2]));
	std::clog << " [" << col_name[3] << "]=" << argv[3];
	it->set_value(DB::g_ModelLessonTeacher.t_id, atol(argv[3]));
	std::clog << " [" << col_name[4] << "]=" << argv[4];
	it->set_value(DB::g_ModelLessonTeacher.t_name, Glib::ustring(argv[4]));
	std::clog << std::endl;
	return 0;
}

int DB::ModelPlan::Callback(void *list_ptr, int argc, char **argv, char **col_name)
{
	if(! list_ptr)
	{
		throw Glib::Error(1, 0, "Null this at ModelPlan::Callback.");
	}
	if(argc != 6)
	{
		throw Glib::Error(1, 0, "Not 6 columns at ModelPlan::Callback.");
	}

	Gtk::ListStore *list_store = reinterpret_cast<Gtk::ListStore*>(list_ptr);
	Gtk::TreeIter it = list_store->append();
	std::clog << " [" << col_name[0] << "]=" << argv[0];
	it->set_value(DB::g_ModelPlan.id, atol(argv[0]));
	std::clog << " [" << col_name[1] << "]=" << argv[1];
	it->set_value(DB::g_ModelPlan.name, Glib::ustring(argv[1]));
	std::clog << " [" << col_name[2] << "]=" << argv[2];
	it->set_value(DB::g_ModelPlan.l_id, atol(argv[2]));
	std::clog << " [" << col_name[3] << "]=" << argv[3];
	it->set_value(DB::g_ModelPlan.t_id, atol(argv[3]));
	std::clog << " [" << col_name[4] << "]=" << argv[4];
	it->set_value(DB::g_ModelPlan.t_name, Glib::ustring(argv[4]));
	std::clog << " [" << col_name[5] << "]=" << argv[5];
	it->set_value(DB::g_ModelPlan.hours, atol(argv[5]));
	std::clog << std::endl;
	return 0;
}

