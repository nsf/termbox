CC ?= cc
AR ?= ar

CFLAGS += -D_XOPEN_SOURCE -fPIC

.PHONY: all demos

all: libtermbox.a libtermbox.so

demos: demo/*.c libtermbox.a
	make -C demo

libtermbox.a: termbox.o utf8.o
	$(AR) rc $@ $^
	-ranlib $@

libtermbox.so: termbox.o utf8.o
	$(CC) $(LDFLAGS) -shared -o $@ $^

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm libtermbox.a
	-rm *.o
	-make -C demo clean

