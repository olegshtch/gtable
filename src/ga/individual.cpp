#include "individual.h"

GA* Individual::ga = 0;

void Individual::Output(std::ostream &os) const
{
	for(size_t i = 0; i < table.size(); i ++)
	{
		const size_t g = ga->ids_g[ga->table_base[i]->g];
		const size_t t = ga->ids_t[ga->table_base[i]->t];
		const size_t l = ga->ids_l[ga->table_base[i]->l];

		const size_t a = ga->ids_a[ADH(table[i]).a];
		const size_t d = ga->ids_d[ADH(table[i]).d];
		const size_t h = ga->ids_h[ADH(table[i]).h];

		os << "(" << 
			ga->db.GetEntityName(DB::g_ModelGroups, g) << "," <<
			ga->db.GetEntityName(DB::g_ModelTeachers, t) << "," <<
			ga->db.GetEntityName(DB::g_ModelLessons, l) << ") -> (" <<
			ga->db.GetEntityName(DB::g_ModelAud, a) << "," <<
			ga->db.GetEntityName(DB::g_ModelDays, d) << "," <<
			ga->db.GetEntityName(DB::g_ModelHours, h) << ")" << std::endl;
	}
}

