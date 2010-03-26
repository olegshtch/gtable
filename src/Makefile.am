SUBDIRS = db
bin_PROGRAMS = table
table_SOURCES = auditorium_frame.cpp conditions.cpp combobox_dialog.cpp entities_list_widget.cpp entry_text_dialog.cpp groups_frame.cpp lessons_widget.cpp lessons_frame.cpp link_list_widget.cpp logbuf.cpp main.cpp main_window.cpp notebook_label_widget.cpp order_dialog.cpp teachers_frame.cpp time_frame.cpp
noinst_HEADERS = auditorium_frame.h conditions.h combobox_dialog.h entities_list_widget.h entry_text_dialog.h groups_frame.h lessons_widget.h lessons_frame.h link_list_widget.h logbuf.h main_window.h notebook_label_widget.h shared.h order_dialog.h teachers_frame.h time_frame.h

AM_CXXFLAGS = \
	-I$(top_srcdir) \
	-DTABLE_LOCALEDIR=\"${TABLE_LOCALEDIR}\" \
	-DGETTEXT_PACKAGE=\"${GETTEXT_PACKAGE}\" \
	$(GLIB_CFLAGS)
table_CXXFLAGS = $(AM_CXXFLAGS)
table_LDADD = \
	$(INTLLIBS) \
	$(GLIB_LIBS) \
	db/libdb.a
