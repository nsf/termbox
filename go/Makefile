include $(GOROOT)/src/Make.inc

TERMBOX_LIB=_cmake_build/libtermbox.a
CGO_LDFLAGS=$(TERMBOX_LIB)
CGO_DEPS=$(TERMBOX_LIB)

TARG=termbox
CGOFILES=termbox.go
GOFILES=structures.$(O).go

CLEANFILES+=_cmake_build structures.$(O).go

include $(GOROOT)/src/Make.pkg

$(TERMBOX_LIB):
	bash make_libtermbox.bash

structures.$(O).go: structures.c
	godefs -g termbox structures.c | gofmt > structures.$(O).go
