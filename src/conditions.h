#ifndef _CONDITIONS_H_
#define _CONDITIONS_H_

#include "db/db.h"

class Conditions
{
public:
	Conditions(DB::DataBase &db);
	~Conditions();
private:
	DB::DataBase &m_DB;
	size_t A, D, H, T, G, L;
	std::vector<size_t> ids_a, ids_d, ids_h, ids_t, ids_g, ids_l;
};

#endif

