#ifndef _ORM_COUNT_H_
#define _ORM_COUNT_H_

#include "expr.h"

namespace ORM
{
	class Count : public Expr<long int>
	{
	public:
		Count()
		{
			m_Expr = "COUNT(*)";
		}
		~Count()
		{
		}
	private:
	};
}

#endif

