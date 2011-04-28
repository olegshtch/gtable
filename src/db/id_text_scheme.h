#ifndef _DB_IDTEXTSCHEME_H_
#define _DB_IDTEXTSCHEME_H_

#include "../orm/scheme.h"

namespace DB
{
	class IdTextScheme : public ORM::Scheme
	{
	public:
		ORM::Field<long int> fId;
		ORM::Field<Glib::ustring> fText;

		IdTextScheme()
			:fId(""),
			fText("")
		{
			add(fId);
			add(fText);
		}
	};

	extern const IdTextScheme g_IdTextScheme;
}

#endif

