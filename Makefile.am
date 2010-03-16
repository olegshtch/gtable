SUBDIRS = src po

@INTLTOOL_DESKTOP_RULE@

INTLTOOL_FILES = \
	intltool-extract.in \
	intltool-merge.in \
	intltool-update.in
EXTRA_DIST = $(INTLTOOL_FILES)
DISTCLEANFILES = \
	intltool-extract \
	intltool-merge \
	intltool-update

clean-local:
	rm -f po/.intltool-merge-cache

