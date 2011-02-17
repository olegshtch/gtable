#ifndef _GA_INDIVIDUAL_H_
#define _GA_INDIVIDUAL_H_

#include <vector>
#include "ga.h"

struct ADH
{
public:
	ADH(long adh)
	{
		a = adh % GA::A;
		adh /= GA::A;
		d = adh % GA::D;
		adh /= GA::D;
		h = adh;
	}
	size_t a, d, h;
/*
	static void SetSizes(size_t A_, size_t D_, size_t H_)
	{
		A = A_;
		D = D_;
		H = H_;
	}
private:
	static size_t A, D, H;*/
};

// особь
class Individual
{
public:
	Individual(size_t gtl, size_t adh)
	{
		table.resize(gtl, -1);
		busy.resize(adh, 0);
	}
	~Individual()
	{
	}

	bool SetTable(size_t index_gtl, long adh_value)
	{
		//check
		if((adh_value != -1) && (busy[adh_value] > 0) && (! ga->multi_aud[ADH(adh_value).a]))
		{
			return false;
		}

		if(table[index_gtl] != -1)
		{
			busy[table[index_gtl]] --;
		}
		table[index_gtl] = adh_value;
		if(table[index_gtl] != -1)
		{
			busy[table[index_gtl]] ++;
		}
		return true;
	}

	const std::vector<signed long>& GetTable() const
	{
		return table;
	}

	const std::vector<signed long>& GetBusy() const
	{
		return busy;
	}

	void Mutation()
	{
		size_t i1 = rand() % table.size();
		size_t i2 = rand() % table.size();
		signed long swap = table[i1];
		table[i1] = table[i2];
		table[i2] = swap;
	}

	void Crossover(const Individual& other)
	{
		for(size_t i = 0; i < table.size() ; i ++)
		{
			if(rand() % 2)
			{
				SetTable(i, other.table[i]);
			}
		}
	}

	static void SetGA(GA *ga_)
	{
		ga = ga_;
	}

	void Output(std::ostream &os) const;
private:
	std::vector<signed long> table; // вектор соответствия занятий и времени с аудиторией.
	std::vector<signed long> busy; // занятие соответствующих аудиторий и времени
	//std::vector<bool> busy_gdh; // занятость групп
	//std::vector<bool> busy_tdh; // занятость преподавателей
	static GA *ga;
};


#endif

