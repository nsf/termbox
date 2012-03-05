include $(GOROOT)/src/Make.inc

TARG=termbox
CGOFILES=termbox.go
CGO_LDFLAGS=-ltermbox

CLEANFILES+=_cmake_build

include $(GOROOT)/src/Make.pkg

html: termbox.go
	gortfm termbox.go
CLEANFILES+=html
