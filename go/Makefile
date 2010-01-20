include $(GOROOT)/src/Make.$(GOARCH)

TERMBOX_LIB=_cmake_build/libtermbox.a
CGO_LDFLAGS=$(TERMBOX_LIB)

TARG=termbox
CGOFILES=\
	 termbox.go

include Make.pkg

$(TERMBOX_LIB):
	bash make_libtermbox.bash

_cgo_.so: $(TERMBOX_LIB)

clean: termbox_clean

termbox_clean:
	rm -rf _cmake_build
