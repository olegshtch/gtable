#include "celleditableforeign.h"
#include "db/db.h"

void CellEditableForeign::start_editing_vfunc(GdkEvent* event)
{
	std::cout << "CellEditableForeign::start_editing_vfunc" << std::endl;
	m_ComboBox.signal_changed().connect(sigc::mem_fun(*this, &CellEditableForeign::OnComboBoxChanged));

	show_all_children();
}

void CellEditableForeign::OnComboBoxChanged()
{
	signal_editing_done_.emit();
}

