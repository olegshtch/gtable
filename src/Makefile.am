SUBDIRS = db ga orm
bin_PROGRAMS = table
table_SOURCES = ga/fitness.cpp \
		ga/ga.cpp \
		ga/individual.cpp \
		logbuf.cpp \
		main.cpp \
		main_window.cpp
noinst_HEADERS = logbuf.h \
		 main_window.h \
		 shared.h

AM_CXXFLAGS = \
	-I$(top_srcdir) \
	-DTABLE_LOCALEDIR=\"${TABLE_LOCALEDIR}\" \
	-DGETTEXT_PACKAGE=\"${GETTEXT_PACKAGE}\" \
	$(GLIB_CFLAGS) \
	-DUIDIR=\"$(pkgdatadir)/ui/\"
table_CXXFLAGS = $(AM_CXXFLAGS)
table_LDADD = \
	$(INTLLIBS) \
	$(GLIB_LIBS) \
	db/libdb.a \
	orm/liborm.a

