#ifndef _SHARED_H_
#define _SHARED_H_

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <glibmm/ustring.h>
#include <glib/gi18n.h>
#include <glibmm/refptr.h>
#include <stdexcept>

/* nice compile-time debug strings */
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__)
#define THROW(a) throw std::runtime_error(a " at " AT)

#define G__FUNCTION__ static_cast<const char*>(__FUNCTION__)
#define G__FILE__ static_cast<const char*>(__FILE__)

template<class T_CppObject> T_CppObject inline *DeRef(Glib::RefPtr<T_CppObject> &ptr)
{
	return ptr.operator->();
}

template <int= sizeof(wchar_t)> struct ustring2w_select;

template <> struct ustring2w_select<2>
{
  typedef gunichar2* gunistr_t;
  gunistr_t (*fun)(const gchar*, glong, glong*, glong*, GError**);
  ustring2w_select(): fun(g_utf8_to_utf16){}
};

template <> struct ustring2w_select<4>
{
  typedef gunichar* gunistr_t;
  gunistr_t (*fun)(const gchar*, glong, glong*, glong*, GError**);
  ustring2w_select(): fun(g_utf8_to_ucs4){}
};

inline wchar_t* UstringConvert(const Glib::ustring& str)
{
	static ustring2w_select<> which;
	return reinterpret_cast<wchar_t*>(which.fun(str.c_str(), -1, 0, 0, 0));
}

inline wchar_t* Utf8Convert(const char* str)
{
	static ustring2w_select<> which;
	return reinterpret_cast<wchar_t*>(which.fun(str, -1, 0, 0, 0));
}

class WChar
{
public:
	WChar(const Glib::ustring& str)
		:m_Data(UstringConvert(str))
	{
	}
	WChar(const char *str)
		:m_Data(Utf8Convert(str))
	{
	}
	~WChar()
	{
		g_free(m_Data);
	}
	operator const wchar_t*() const
	{
		return m_Data;
	}
private:
	wchar_t *m_Data;
};

template <int= sizeof(wchar_t)> struct w2ustring_select;

template <> struct w2ustring_select<2>
{
  typedef gunichar2 const* gunistr_t;
  gchar* (*fun)(gunistr_t, glong, glong*, glong*, GError**);
  w2ustring_select(): fun(g_utf16_to_utf8){}
};

template <> struct w2ustring_select<4>
{
  typedef gunichar const * gunistr_t;
  gchar* (*fun)(gunistr_t, glong, glong*, glong*, GError**);
  w2ustring_select(): fun(g_ucs4_to_utf8){}
};

Glib::ustring inline w2ustring(const wchar_t *w)
{
  static w2ustring_select<> which;
  typedef w2ustring_select<>::gunistr_t whichstr_t;
  whichstr_t whatever= reinterpret_cast<whichstr_t>(w);
  gchar* utf8= which.fun(whatever, -1, 0, 0, 0);
  Glib::ustring u(utf8); g_free(utf8);
  return u;
}

#endif

