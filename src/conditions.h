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
};

#endif

