prefix=/usr/local
bindir=$(prefix)/bin
includedir=$(prefix)/include
libdir=$(prefix)/lib
sysconfdir=$(prefix)/etc

LIBSRC = $(sort $(wildcard src/*.c))
DEMOSRC = $(sort $(wildcard src/demo/*.c))

SRCS = $(LIBSRC) $(DEMOSRC)
OBJS = $(SRCS:.c=.o)
LIBOBJS = $(LIBSRC:.c=.o)
DEMOOBJS = $(DEMOSRC:.c=.o)

HEADERS = src/termbox.h
ALL_INCLUDES = $(HEADERS)

SHARED_LIBS=libtermbox.so
ALL_LIBS=libtermbox.a
ALL_DEMOS=keyboard_demo

CFLAGS=-fPIC -O0 -g -std=gnu99 -D_GNU_SOURCE

AR      = $(CROSS_COMPILE)ar
RANLIB  = $(CROSS_COMPILE)ranlib


-include config.mak

BUILDCFLAGS=$(CFLAGS)

all: $(ALL_LIBS) $(ALL_DEMOS) $(SHARED_LIBS)

install: $(ALL_LIBS:lib/%=$(DESTDIR)$(libdir)/%) $(ALL_INCLUDES:include/%=$(DESTDIR)$(includedir)/%)

clean:
	rm -f $(ALL_LIBS)
	rm -f $(ALL_DEMOS)
	rm -f $(OBJS)

%.o: %.c
	$(CC) $(BUILDCFLAGS) -c -o $@ $<

libtermbox.a: $(LIBOBJS)
	rm -f $@
	$(AR) rc $@ $(LIBOBJS)
	$(RANLIB) $@

libtermbox.so: $(LIBOBJS)
	gcc -shared -o $(SHARED_LIBS) $(LIBOBJS)

keyboard_demo: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

$(DESTDIR)$(libdir)/%.a: %.a
	install -D -m 755 $< $@

$(DESTDIR)$(includedir)/%: include/%
	install -D -m 644 $< $@

.PHONY: all clean install



