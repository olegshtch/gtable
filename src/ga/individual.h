#ifndef _GA_INDIVIDUAL_H_
#define _GA_INDIVIDUAL_H_

#include <vector>

struct ADH
{
public:
	ADH(long adh)
	{
		a = adh % A;
		adh /= A;
		d = adh % D;
		adh /= D;
		h = adh;
	}
	size_t a, d, h;

	static void SetSizes(size_t A_, size_t D_, size_t H_)
	{
		A = A_;
		D = D_;
		H = H_;
	}
private:
	static size_t A, D, H;
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
		if((adh_value != -1) && (busy[adh_value] > 0) && (! multi_aud[ADH(adh_value).a]))
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

	static void SetMultiAud(const std::vector<bool>& multi_aud_)
	{
		multi_aud = multi_aud_;
	}
private:
	std::vector<signed long> table; // вектор соответствия занятий и времени с аудиторией.
	std::vector<signed long> busy; // занятие соответствующих аудиторий и времени
	static std::vector<bool> multi_aud; // многопоточные аудитории
};


#endif

