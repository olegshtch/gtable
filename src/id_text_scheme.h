#ifndef _IDTEXTSCHEME_H_
#define _IDTEXTSCHEME_H_

#include "orm/scheme.h"

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

#endif

