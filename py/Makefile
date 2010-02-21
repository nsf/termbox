
all: termbox.so

CFLAGS=-O3 -fPIC -march=native

termbox.so: termbox.o lib_input.o lib_termbox.o lib_utf8.o lib_ringbuffer.o lib_term.o
	gcc $(CFLAGS) -I/usr/include/python2.6 -I.. -shared -o termbox.so $^
	strip -s termbox.so

lib_%.o: ../%.c
	gcc $(CFLAGS) -fvisibility=hidden -I.. -o $@ -c $<

termbox.o: termbox.pyx
	pyrexc termbox.pyx
	gcc $(CFLAGS) -I/usr/include/python2.6 -I.. -o termbox.o -c termbox.c

clean:
	rm -rf *.o *.so termbox.c

