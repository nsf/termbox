CC ?= cc
AR ?= ar

CFLAGS := -Wall -D_XOPEN_SOURCE

.PHONY: all demos

all: libtermbox.a

demos: demo/*.c libtermbox.a
	make -C demo

libtermbox.a: termbox.o utf8.o
	$(AR) rc $@ $^
	-ranlib $@

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm libtermbox.a
	-rm *.o
	-make -C demo clean

