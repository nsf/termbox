#pragma once

struct bytebuffer {
	char *buf;
	int len;
	int cap;
};

void bytebuffer_init(struct bytebuffer *b, int cap);
void bytebuffer_free(struct bytebuffer *b);
void bytebuffer_clear(struct bytebuffer *b);
void bytebuffer_resize(struct bytebuffer *b, int len);
void bytebuffer_puts(struct bytebuffer *b, const char *str);
void bytebuffer_append(struct bytebuffer *b, const char *data, int len);

// remove n first bytes, shifting the rest backwards
void bytebuffer_truncate(struct bytebuffer *b, int n);

// writes the contents of the buffer to the given fd and then clears the buffer
void bytebuffer_flush(struct bytebuffer *b, int fd);
