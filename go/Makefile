include $(GOROOT)/src/Make.inc

TERMBOX_LIB=_cmake_build/libtermbox.a
CGO_LDFLAGS=$(TERMBOX_LIB)
CGO_DEPS=$(TERMBOX_LIB)

TARG=termbox
CGOFILES=termbox.go

CLEANFILES+=_cmake_build

include $(GOROOT)/src/Make.pkg

$(TERMBOX_LIB):
	bash make_libtermbox.bash

html: termbox.go
	gortfm termbox.go
CLEANFILES+=html
