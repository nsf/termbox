CC ?= cc

LDFLAGS := -Wl,-Bstatic -L../ -ltermbox -Wl,-Bdynamic

.PHONY: all clean

all: output paint keyboard

output: output.c
	$(CC) -o $@ $< $(LDFLAGS)

paint: paint.c
	$(CC) -o $@ $< $(LDFLAGS)

keyboard: keyboard.c
	$(CC) -o $@ $< $(LDFLAGS)

clean:
	-rm output
	-rm paint
	-rm keyboard

