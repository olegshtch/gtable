# == initialization ==

AC_INIT([Table],
	[0.0.1],
	[olegshtch@yandex.ru],
	[table])

AC_CONFIG_SRCDIR([src/main.cpp])

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([-Wall -Werror dist-bzip2 foreign])

# == basic compiler settings ==

AC_PROG_CXX

AC_PROG_CXX_C_O

AC_LANG(C++)

AC_PROG_RANLIB

CXXFLAGS="-Wall -Wextra -pedantic -O0 -ggdb3"

# ====== internationalization i18n ======

IT_PROG_INTLTOOL([0.40.4])

GETTEXT_PACKAGE=table

#AC_DEFINE([GETTEXT_PACKAGE], [table], [Description])

AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], ["$GETTEXT_PACKAGE"], [The domain to use with gettext])

AM_GLIB_GNU_GETTEXT

#AM_GNU_GETTEXT

#AM_GNU_GETTEXT_VERSION

AC_SUBST(GETTEXT_PACKAGE)

TABLE_LOCALEDIR=[${datadir}/locale]

AC_SUBST(TABLE_LOCALEDIR)

PKG_CHECK_MODULES([GLIB],[glib-2.0 >= 2.18])

# == checks for libraries ==

PKG_CHECK_MODULES([GTHREAD],[gthread-2.0],[LDFLAGS="$GTHREAD_LIBS $LDFLAGS";CXXFLAGS="$GTHREAD_CFLAGS $CXXFLAGS"],[AC_MSG_ERROR([Sorry, but you need GThread library])])
PKG_CHECK_MODULES([GTKMM],[gtkmm-2.4],[LDFLAGS="$GTKMM_LIBS $LDFLAGS";CXXFLAGS="$GTKMM_CFLAGS $CXXFLAGS"],[AC_MSG_ERROR([Sorry, but you need GTKmm library])])
PKG_CHECK_MODULES([SQLITE],[sqlite3],[LDFLAGS="$SQLITE_LIBS $LDFLAGS";CXXFLAGS="$SQLITE_CFLAGS $CXXFLAGS"],[AC_MSG_ERROR([Sorry, but you need SQLite3 library])])

# == checks for header files ==

# == checks for typedefs, structures and compiler characteristics ==

# == checks for library functions ==

# == generate files ==

AC_CONFIG_FILES([
    Makefile
    docs/Makefile
    src/Makefile
    src/3rd_party/Makefile
    src/db/Makefile
    src/ga/Makefile
    src/orm/Makefile
    po/Makefile.in
    win32/Makefile
    ui/Makefile
])

AC_OUTPUT

