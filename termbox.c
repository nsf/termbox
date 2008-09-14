#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <termios.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "term.h"
#include "termbox.h"

struct cellbuf {
	unsigned int width;
	unsigned int height;
	struct tb_cell *cells;
};

#define CELL(buf, x, y) (buf)->cells[(y) * (buf)->width + (x)]

static struct termios orig_tios;

static struct cellbuf back_buffer;
static struct cellbuf front_buffer;
static unsigned int termw;
static unsigned int termh;

static int inputmode = TB_INPUT_ESC;

static struct ringbuffer inbuf;

static FILE *out;
static FILE *in;

static volatile int sigwinch_r;

static void cellbuf_init(struct cellbuf *buf, unsigned int width, unsigned int height);
static void cellbuf_resize(struct cellbuf *buf, unsigned int width, unsigned int height);
static void cellbuf_clear(struct cellbuf *buf);
static void cellbuf_free(struct cellbuf *buf);

static void update_term_size();
static int utf8_unicode_to_char(char *out, uint32_t c);
static void send_attr(uint16_t fg, uint16_t bg);
static void send_char(unsigned int x, unsigned int y, uint32_t c);
static void send_clear();
static void sigwinch_handler(int xxx);
static void check_sigwinch();

static void fill_inbuf();

/* -------------------------------------------------------- */

int tb_init()
{
	out = fopen("/dev/tty", "w");
	in = fopen("/dev/tty", "r");

	if (!out || !in) 
		return TB_EFAILED_TO_OPEN_TTY;
	
	if (init_term() < 0)
		return TB_EUNSUPPORTED_TERMINAL;

	signal(SIGWINCH, sigwinch_handler);

	tcgetattr(fileno(out), &orig_tios);
	struct termios tios;
	memset(&tios, 0, sizeof(tios));
	
	tios.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
                           | INLCR | IGNCR | ICRNL | IXON);
	tios.c_oflag &= ~OPOST;
	tios.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	tios.c_cflag &= ~(CSIZE | PARENB);
	tios.c_cflag |= CS8;
	tios.c_cc[VMIN] = 0;
	tios.c_cc[VTIME] = 0;
	tcsetattr(fileno(out), TCSAFLUSH, &tios);

	fputs(funcs[T_ENTER_CA], out);
	fputs(funcs[T_ENTER_KEYPAD], out);
	fputs(funcs[T_HIDE_CURSOR], out);
	fputs(funcs[T_CLEAR_SCREEN], out);

	fflush(out);
	update_term_size();
	cellbuf_init(&back_buffer, termw, termh);
	cellbuf_init(&front_buffer, termw, termh);
	cellbuf_clear(&back_buffer);
	cellbuf_clear(&front_buffer);
	init_ringbuffer(&inbuf, 4096);

	return 0;
}

void tb_shutdown()
{
	fputs(funcs[T_SHOW_CURSOR], out);
	fputs(funcs[T_SGR0], out);
	fputs(funcs[T_CLEAR_SCREEN], out);
	fputs(funcs[T_EXIT_CA], out);
	fputs(funcs[T_EXIT_KEYPAD], out);
	fflush(out);
	tcsetattr(fileno(out), TCSAFLUSH, &orig_tios);

	fclose(out);
	fclose(in);

	cellbuf_free(&back_buffer);
	cellbuf_free(&front_buffer);
	free_ringbuffer(&inbuf);
}

void tb_present()
{
	unsigned int x,y;
	uint16_t lastfg = 0xFFFF;
	uint16_t lastbg = 0xFFFF;
	struct tb_cell *back, *front;

	check_sigwinch();

	for (y = 0; y < front_buffer.height; ++y) {
		back = &CELL(&back_buffer, 0, y);
		front = &CELL(&front_buffer, 0, y);
		for (x = 0; x < front_buffer.width; ++x) {
			back = &CELL(&back_buffer, x, y);
			front = &CELL(&front_buffer, x, y);
			/* what's faster? */
/*			if (*((uint32_t*)back) == *((uint32_t*)front) && 
			    *((uint32_t*)&back->fg) == *((uint32_t*)&front->fg)) */
			if (memcmp(back, front, sizeof(struct tb_cell)) == 0)
				continue;
			if (lastfg != back->fg || lastbg != back->bg) {
				lastfg = back->fg;
				lastbg = back->bg;
				send_attr(lastfg, lastbg);
			}
			send_char(x, y, back->ch);
			memcpy(front, back, sizeof(struct tb_cell));
		}
	}
	fflush(out);
}

void tb_put_cell(unsigned int x, unsigned int y, const struct tb_cell *cell)
{
	if (x >= back_buffer.width || y >= back_buffer.height)
		return;
	CELL(&back_buffer, x, y) = *cell;
}

void tb_change_cell(unsigned int x, unsigned int y, uint32_t ch, uint16_t fg, uint16_t bg)
{
	struct tb_cell c = {ch, fg, bg};
	tb_put_cell(x, y, &c);
}

int tb_poll_event(struct tb_key_event *event)
{
	int i;
	char buf[32];
	fd_set events;
	memset(event, 0, sizeof(struct tb_key_event));

	/* try to extract event from input buffer, return on success */
	if (extract_event(event, &inbuf, inputmode) == 0)
		return 1;

	/* it looks like input buffer is empty, wait for input and fill it */

	while (1) {
		FD_ZERO(&events);
		FD_SET(fileno(in), &events);
		select(fileno(in)+1, &events, 0, 0, 0);

		if (FD_ISSET(fileno(in), &events)) {
			int r = fread(buf, 1, 32, in);
			/* if it's zero read, this is a resize message */
			if (r == 0)
				continue;
			/* if there is no free space in input buffer, return error */
			if (ringbuffer_free_space(&inbuf) < r)
				return -1;
			/* fill buffer */
			ringbuffer_push(&inbuf, buf, r);
			if (extract_event(event, &inbuf, inputmode) == 0)
				return 1;
		}
	}
}

unsigned int tb_width() 
{
	return termw;
}

unsigned int tb_height()
{
	return termh;
}

void tb_clear()
{
	check_sigwinch();
	cellbuf_clear(&back_buffer);
}

void tb_select_input_mode(int mode)
{
	inputmode = mode;
}

/* -------------------------------------------------------- */
	
static void cellbuf_init(struct cellbuf *buf, unsigned int width, unsigned int height)
{
	buf->cells = malloc(sizeof(struct tb_cell) * width * height);
	assert(buf->cells);
	buf->width = width;
	buf->height = height;
}

static void cellbuf_resize(struct cellbuf *buf, unsigned int width, unsigned int height)
{
	if (buf->width == width && buf->height == height)
		return;

	unsigned int oldw = buf->width;
	unsigned int oldh = buf->height;
	struct tb_cell *oldcells = buf->cells;

	cellbuf_init(buf, width, height);
	cellbuf_clear(buf);

	unsigned int minw = (width < oldw) ? width : oldw;
	unsigned int minh = (height < oldh) ? height : oldh;
	unsigned int i;

	for (i = 0; i < minh; ++i) {
		struct tb_cell *csrc = oldcells + (i * oldw);
		struct tb_cell *cdst = buf->cells + (i * width);
		memcpy(cdst, csrc, sizeof(struct tb_cell) * minw);
	}

	free(oldcells);
}

static void cellbuf_clear(struct cellbuf *buf)
{
	unsigned int i;
	unsigned int ncells = buf->width * buf->height;

	for (i = 0; i < ncells; ++i) {
		buf->cells[i].ch = ' ';
		buf->cells[i].fg = TB_WHITE;
		buf->cells[i].bg = TB_BLACK;
	}
}

static void cellbuf_free(struct cellbuf *buf)
{
	free(buf->cells);
}

static void update_term_size()
{
	struct winsize sz;
	memset(&sz, 0, sizeof(sz));
	
	ioctl(fileno(out), TIOCGWINSZ, &sz);

	termw = sz.ws_col;
	termh = sz.ws_row;
}

static int utf8_unicode_to_char(char *out, uint32_t c)
{
	uint len = 0;    
	int first;
	int i;

	if (c < 0x80) {
		first = 0;
		len = 1;	
	} else if (c < 0x800) {
		first = 0xc0;
		len = 2;
	} else if (c < 0x10000) {
		first = 0xe0;
		len = 3;
	} else if (c < 0x200000) {
		first = 0xf0;
		len = 4;
	} else if (c < 0x4000000) {
		first = 0xf8;
		len = 5;
	} else {
		first = 0xfc;
		len = 6;
	}

	for (i = len - 1; i > 0; --i) {
		out[i] = (c & 0x3f) | 0x80;
		c >>= 6;
	}
	out[0] = c | first;

	return len;
}

static void send_attr(uint16_t fg, uint16_t bg)
{
	fputs(funcs[T_SGR0], out);
	fprintf(out, funcs[T_SGR], fg & 0x0F, bg & 0x0F);
	if (fg & TB_BOLD)
		fputs(funcs[T_BOLD], out);
	if (bg & TB_BOLD)
		fputs(funcs[T_BLINK], out);
}

static void send_char(unsigned int x, unsigned int y, uint32_t c)
{
	char buf[7];
	int bw = utf8_unicode_to_char(buf, c);
	buf[bw] = '\0';
	fprintf(out, funcs[T_MOVE_CURSOR], y+1, x+1);
	fputs(buf, out);
}

static void send_clear()
{
	send_attr(TB_WHITE, TB_BLACK);
	fputs(funcs[T_CLEAR_SCREEN], out);
	fflush(out);
}

static void sigwinch_handler(int xxx)
{
	sigwinch_r = 1;
}

static void check_sigwinch()
{
	if (sigwinch_r) {
		update_term_size();
		cellbuf_resize(&back_buffer, termw, termh);
		cellbuf_resize(&front_buffer, termw, termh);
		cellbuf_clear(&front_buffer);

		send_clear();
		
		sigwinch_r = 0;
	}
}

