#pragma once

#include "termbox.h"

// This is simply a termbox mapping for a C++, it uses the same functionality,
// but resides in its own namespace.
namespace termbox {

// The only reason why I use enum here instead of defines, because enum has
// scope (resides in a namespace).
enum {

// keys
	key_f1		= (0xFFFF-0),
	key_f2		= (0xFFFF-1),
	key_f3		= (0xFFFF-2),
	key_f4		= (0xFFFF-3),
	key_f5		= (0xFFFF-4),
	key_f6		= (0xFFFF-5),
	key_f7		= (0xFFFF-6),
	key_f8		= (0xFFFF-7),
	key_f9		= (0xFFFF-8),
	key_f10		= (0xFFFF-9),
	key_f11		= (0xFFFF-10),
	key_f12		= (0xFFFF-11),
	key_insert	= (0xFFFF-12),
	key_delete	= (0xFFFF-13),
	key_home	= (0xFFFF-14),
	key_end		= (0xFFFF-15),
	key_pgup	= (0xFFFF-16),
	key_pgdn	= (0xFFFF-17),
	key_arrow_up	= (0xFFFF-18),
	key_arrow_down	= (0xFFFF-19),
	key_arrow_left	= (0xFFFF-20),
	key_arrow_right	= (0xFFFF-21),

	key_ctrl_tilde		= 0x00,
	key_ctrl_2		= 0x00,
	key_ctrl_a		= 0x01,
	key_ctrl_b		= 0x02,
	key_ctrl_c		= 0x03,
	key_ctrl_d		= 0x04,
	key_ctrl_e		= 0x05,
	key_ctrl_f		= 0x06,
	key_ctrl_g		= 0x07,
	key_backspace		= 0x08,
	key_ctrl_h		= 0x08,
	key_tab			= 0x09,
	key_ctrl_i		= 0x09,
	key_ctrl_j		= 0x0a,
	key_ctrl_k		= 0x0b,
	key_ctrl_l		= 0x0c,
	key_enter		= 0x0d,
	key_ctrl_m		= 0x0d,
	key_ctrl_n		= 0x0e,
	key_ctrl_o		= 0x0f,
	key_ctrl_p		= 0x10,
	key_ctrl_q		= 0x11,
	key_ctrl_r		= 0x12,
	key_ctrl_s		= 0x13,
	key_ctrl_t		= 0x14,
	key_ctrl_u		= 0x15,
	key_ctrl_v		= 0x16,
	key_ctrl_w		= 0x17,
	key_ctrl_x		= 0x18,
	key_ctrl_y		= 0x19,
	key_ctrl_z		= 0x1a,
	key_esc			= 0x1b,
	key_ctrl_lsq_bracket	= 0x1b,
	key_ctrl_3		= 0x1b,
	key_ctrl_4		= 0x1c,
	key_ctrl_backslash	= 0x1c,
	key_ctrl_5		= 0x1d,
	key_ctrl_rsq_bracket	= 0x1d,
	key_ctrl_6		= 0x1e,
	key_ctrl_7		= 0x1f,
	key_ctrl_slash		= 0x1f,
	key_ctrl_underscore	= 0x1f,
	key_space		= 0x20,
	key_backspace2		= 0x7f,
	key_ctrl_8		= 0x7f,

// modifier
	mod_alt	= 0x01,

// attributes & colors
	black		= 0x00,
	red		= 0x01,
	green		= 0x02,
	yellow		= 0x03,
	blue		= 0x04,
	magenta		= 0x05,
	cyan		= 0x06,
	white		= 0x07,
	bold		= 0x10,
	underline	= 0x20,

// initialization errors
	eunsupported_terminal	= -1,
	efailed_to_open_tty	= -2,
	epipe_trap_error	= -3,

// special cursor position
	hide_cursor = -1,

// event types
	event_key = 1,
	event_resize = 2,

// input modes
	input_esc = 1,
	input_alt = 2,
};

typedef struct tb_cell cell;
typedef struct tb_event event;

inline int init() { return tb_init(); }
inline void shutdown() { return tb_shutdown(); }

inline unsigned int width() { return tb_width(); }
inline unsigned int height() { return tb_height(); }

inline void clear() { tb_clear(); }
inline void present() { tb_present(); }

inline void set_cursor(int cx, int cy) { tb_set_cursor(cx, cy); }

inline void put_cell(unsigned int x, unsigned int y, const cell &c) { tb_put_cell(x,y,&c); }
inline void change_cell(unsigned int x, unsigned int y, uint32_t ch, uint16_t fg, uint16_t bg) { tb_change_cell(x,y,ch,fg,bg); }
inline void blit(unsigned int x, unsigned int y, unsigned int w, unsigned int h, const cell *cells) { tb_blit(x,y,w,h,cells); }

inline int select_input_mode(int mode) { return tb_select_input_mode(mode); }

inline int peek_event(event &e, unsigned int timeout = 0) { return tb_peek_event(&e, timeout); }
inline int poll_event(event &e) { return tb_poll_event(&e); }

using ::utf8_char_length;
using ::utf8_char_to_unicode;
using ::utf8_unicode_to_char;

} // namespace
