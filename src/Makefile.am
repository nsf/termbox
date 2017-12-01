AM_CPPFLAGS = \
		   -I$(top_srcdir)

libtermbox_la_SOURCES = termbox.c utf8.c
libtermbox_la_LDFLAGS = \
                        -Wl,-soname,libtermbox.so.$(TERMBOX_VERSION_MAJOR) \
                        -version-info $(TERMBOX_VERSION_MAJOR):$(TERMBOX_VERSION_MINOR):$(TERMBOX_VERSION_PATCH)
libtermbox_la_HEADERS = termbox.h
libtermbox_ladir = $(top_srcdir)/src

lib_LTLIBRARIES = libtermbox.la

