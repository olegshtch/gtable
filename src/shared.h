#ifndef _SHARED_H_
#define _SHARED_H_

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <glibmm/ustring.h>
#include <glib/gi18n.h>

#define G__FUNCTION__ static_cast<const char*>(__FUNCTION__)
#define G__FILE__ static_cast<const char*>(__FILE__)

template<class T_CppObject> T_CppObject inline *DeRef(Glib::RefPtr<T_CppObject> &ptr)
{
	return ptr.operator->();
}

#endif

