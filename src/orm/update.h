#ifndef _ORM_UPDATE_H_
#define _ORM_UPDATE_H_

#include "query.h"
#include "where.h"

namespace ORM
{
	class UpdateBase : public QueryBase
	{
	public:
		UpdateBase(Connection& db, const Table& table)
			:QueryBase(db, "UPDATE " + table.GetTableName())
		{
		}
		~UpdateBase()
		{
		}

		UpdateBase* Set(const FieldBase& field, const Gtk::TreeIter& row)
		{
			m_Query += " SET " + field.GetSmallFieldName() + "=" + field.GetStrValue(row);
			return this;
		}
		template<class T> UpdateBase* Set(const Field<T>& field, const T& value)
		{
			m_Query += " SET " + field.GetSmallFieldName() + "=" + Field<T>::ToString(value);
			return this;
		}

		void Where(const WhereBase& where)
		{
			m_Query += " WHERE " + where.GetQuery();
		}
	private:
	};
}

#endif

