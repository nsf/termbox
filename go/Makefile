include $(GOROOT)/src/Make.inc

TERMBOX_LIB=_cmake_build/libtermbox.a
TERMBOX_OFILES=input.c.o ringbuffer.c.o term.c.o termbox.c.o utf8.c.o

TARG=termbox
CGOFILES=termbox.go
CGO_OFILES=$(TERMBOX_OFILES)

CLEANFILES+=_cmake_build

include $(GOROOT)/src/Make.pkg

$(TERMBOX_OFILES):
	bash make_libtermbox.bash
	ar x $(TERMBOX_LIB)

html: termbox.go
	gortfm termbox.go
CLEANFILES+=html
