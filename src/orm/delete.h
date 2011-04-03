#ifndef _ORM_DELETE_H_
#define _ORM_DELETE_H_

#include "query.h"

namespace ORM
{
	class DeleteBase : public QueryBase
	{
	public:
		DeleteBase(Connection &db, const Table& table)
			:QueryBase(db, "DELETE FROM " + table.GetTableName())
		{
		}
		~DeleteBase()
		{
		}

		void Where(const WhereBase& where)
		{
			m_Query += " WHERE " + where.GetQuery();
		}
	private:
	};
}

#endif

