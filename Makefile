
prefix=/usr/local
libdir=$(prefix)/lib
includedir=$(prefix)/include

CFLAGS=-std=c99 -D_POSIX_SOURCE -Wall -Wextra -Wno-unused -O2

SRC=termbox.c term.c input.c ringbuffer.c utf8.c
OBJ=$(subst .c,.o,$(SRC))
LIB=libtermbox.a
INC=termbox.h
SHARED=libtermbox.so

TEST_CFLAGS=-std=c99 -Wall -Wextra -Wno-unused -O2 -I.
TEST_LDFLAGS=-L.
TEST_LDADD=-ltermbox

TEST_SRC=keyboard.c
TEST_BIN=$(subst .c,,$(TEST_SRC))

.SUFFIXES: .a .c .h .o
.PHONY: all clean install test uninstall

all: $(LIB) $(SHARED)

test: $(TEST_BIN)

$(LIB): $(OBJ)
	$(AR) cr $@ $^
	ranlib $@

$(SHARED):
	$(CC) $(SRC) -shared -o $(SHARED)

clean:
	$(RM) $(LIB) $(TEST_BIN) $(SHARED) $(wildcard *.o)

install: $(LIB)
	install -m 644 $(INC) $(includedir)
	install -m 644 $(LIB) $(libdir)
	install -m 644 $(SHARED) $(libdir)

uninstall:
	$(RM) $(libdir)/$(LIB)
	$(RM) $(includedir)/$(INC)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

%: %.c $(LIB)
	$(CC) $(TEST_CFLAGS) $(TEST_LDFLAGS) -o $@ $< $(TEST_LDADD)
