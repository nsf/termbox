#ifndef TERMBOX_TERM_H
#define TERMBOX_TERM_H

#include "termbox.h"
#include "bytebuffer.h"

enum {
	T_ENTER_CA,
	T_EXIT_CA,
	T_SHOW_CURSOR,
	T_HIDE_CURSOR,
	T_CLEAR_SCREEN,
	T_SGR0,
	T_UNDERLINE,
	T_BOLD,
	T_BLINK,
	T_REVERSE,
	T_ENTER_KEYPAD,
	T_EXIT_KEYPAD,
	T_FUNCS_NUM,
};

#define EUNSUPPORTED_TERM -1

int init_term(void);
void shutdown_term(void);

/* true on success, false on failure */
bool extract_event(struct tb_event *event, struct bytebuffer *inbuf, int inputmode);

extern const char **keys;
extern const char **funcs;

#endif
