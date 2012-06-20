#include <string.h>
#include <stdio.h>
#include "memstream.h"

void memstream_init(struct memstream *s, FILE* fd, void* buffer, size_t len) {
	s->file = fd,
	s->data = buffer;
	s->pos = 0;
	s->capa = len;
}

void memstream_flush(struct memstream *s) {
	fwrite(s->data, s->pos, 1, s->file);
	fflush(s->file);
	s->pos = 0;
}

void memstream_write(struct memstream *s, void* source, size_t len) {
	unsigned char* data = source;
	if(s->pos + len > s->capa) memstream_flush(s);
	memcpy(s->data + s->pos, data, len);
	s->pos += len;
}

void memstream_puts(struct memstream *s, const char* str) {
	memstream_write(s, (void*) str, strlen(str));
}
