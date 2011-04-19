#include "plan_sheet.h"

PlanSheet::PlanSheet()
{
}

PlanSheet::PlanSheet(GtkTreeView *cobject, const Glib::RefPtr<Gtk::Builder> &builder)
	:Glib::ObjectBase(typeid(PlanSheet)),
	Sheet(cobject, builder)
{
}

PlanSheet::~PlanSheet()
{
}

void PlanSheet::add_empty_line()
{
}

void PlanSheet::remove_line()
{
}

void PlanSheet::update_model()
{
}

void PlanSheet::set_speciality(long int id_speciality)
{

}
