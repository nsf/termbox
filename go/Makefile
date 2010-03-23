include $(GOROOT)/src/Make.$(GOARCH)

TERMBOX_LIB=_cmake_build/libtermbox.a
CGO_LDFLAGS=$(TERMBOX_LIB)

GOFILES:=\
	structures.$(O).go

TARG=termbox
CGOFILES=\
	 termbox.go

CLEANFILES+=_cmake_build structures.$(O).go

include $(GOROOT)/src/Make.pkg

$(TERMBOX_LIB):
	bash make_libtermbox.bash

_cgo_.so: $(TERMBOX_LIB)

structures.$(O).go: structures.c
	godefs -g termbox structures.c > structures.$(O).go
	gofmt -w structures.$(O).go
