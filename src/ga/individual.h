#ifndef _GA_INDIVIDUAL_H_
#define _GA_INDIVIDUAL_H_

#include <vector>

// особь
class Individual
{
public:
	Individual();
	~Individual();
private:
	std::vector<short> table; // вектор соответствия занятий и времени с аудиторией.
	std::vector<short> busy; // занятие соответсвующих аудиторий
};


#endif

