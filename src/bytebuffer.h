#pragma once

struct bytebuffer {
	char *buf;
	int len;
	int cap;
};

void init_bytebuffer(struct bytebuffer *b, int cap);
void free_bytebuffer(struct bytebuffer *b);
void clear_bytebuffer(struct bytebuffer *b);
void resize_bytebuffer(struct bytebuffer *b, int len);
void bytebuffer_puts(struct bytebuffer *b, const char *str);
void bytebuffer_append(struct bytebuffer *b, const char *data, int len);

// remove n first bytes, shifting the rest backwards
void bytebuffer_truncate(struct bytebuffer *b, int n);

// writes the contents of the buffer to the given fd and then clears the buffer
void flush_bytebuffer(struct bytebuffer *b, int fd);
