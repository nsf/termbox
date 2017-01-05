struct bytebuffer {
	char *buf;
	int len;
	int cap;
};

static void bytebuffer_reserve(struct bytebuffer *b, int cap) {
	if (b->cap >= cap) {
		return;
	}

	// prefer doubling capacity
	if (b->cap * 2 >= cap) {
		cap = b->cap * 2;
	}

	char *newbuf = realloc(b->buf, cap);
	b->buf = newbuf;
	b->cap = cap;
}

static void bytebuffer_init(struct bytebuffer *b, int cap) {
	b->cap = 0;
	b->len = 0;
	b->buf = 0;

	if (cap > 0) {
		b->cap = cap;
		b->buf = malloc(cap); // just assume malloc works always
	}
}

static void bytebuffer_free(struct bytebuffer *b) {
	if (b->buf)
		free(b->buf);
}

static void bytebuffer_clear(struct bytebuffer *b) {
	b->len = 0;
}

static void bytebuffer_append(struct bytebuffer *b, const char *data, int len) {
	bytebuffer_reserve(b, b->len + len);
	memcpy(b->buf + b->len, data, len);
	b->len += len;
}

static void bytebuffer_puts(struct bytebuffer *b, const char *str) {
	bytebuffer_append(b, str, strlen(str));
}

static void bytebuffer_resize(struct bytebuffer *b, int len) {
	bytebuffer_reserve(b, len);
	b->len = len;
}

static void bytebuffer_flush(struct bytebuffer *b, int fd) {
	write(fd, b->buf, b->len);
	bytebuffer_clear(b);
}

static void bytebuffer_truncate(struct bytebuffer *b, int n) {
	if (n <= 0)
		return;
	if (n > b->len)
		n = b->len;
	const int nmove = b->len - n;
	memmove(b->buf, b->buf+n, nmove);
	b->len -= n;
}
