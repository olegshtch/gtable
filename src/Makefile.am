SUBDIRS = 3rd_party \
	  db \
	  ga \
	  orm
bin_PROGRAMS = table
table_SOURCES = 3rd_party/BasicExcel.cpp \
		3rd_party/ExcelFormat.cpp \
		ga/fitness.cpp \
		ga/ga.cpp \
		ga/graph.cpp \
		ga/individual.cpp \
		celleditableforeign.cpp \
		cellrendererforeign.cpp \
		listview.cpp \
		logbuf.cpp \
		main.cpp \
		main_window.cpp \
		plan_sheet.cpp \
		sheet.cpp \
		teaching_lesson.cpp
noinst_HEADERS = celleditableforeign.h \
		 cellrendererforeign.h \
		 line_editable.h \
		 listview.h \
		 logbuf.h \
		 main_window.h \
		 plan_sheet.h \
		 shared.h \
		 sheet.h \
		 teaching_lesson.h

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

