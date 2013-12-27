#pragma once

#include <stdbool.h>
#include <stdint.h>

// for shared objects
#if __GNUC__ >= 4
 #define SO_IMPORT __attribute__((visibility("default")))
#else
 #define SO_IMPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Key constants. See also struct tb_event's key field.

// These are a safe subset of terminfo keys, which exist on all popular
// terminals. Termbox uses only them to stay truly portable.
#define TB_KEY_F1		(0xFFFF-0)
#define TB_KEY_F2		(0xFFFF-1)
#define TB_KEY_F3		(0xFFFF-2)
#define TB_KEY_F4		(0xFFFF-3)
#define TB_KEY_F5		(0xFFFF-4)
#define TB_KEY_F6		(0xFFFF-5)
#define TB_KEY_F7		(0xFFFF-6)
#define TB_KEY_F8		(0xFFFF-7)
#define TB_KEY_F9		(0xFFFF-8)
#define TB_KEY_F10		(0xFFFF-9)
#define TB_KEY_F11		(0xFFFF-10)
#define TB_KEY_F12		(0xFFFF-11)
#define TB_KEY_INSERT		(0xFFFF-12)
#define TB_KEY_DELETE		(0xFFFF-13)
#define TB_KEY_HOME		(0xFFFF-14)
#define TB_KEY_END		(0xFFFF-15)
#define TB_KEY_PGUP		(0xFFFF-16)
#define TB_KEY_PGDN		(0xFFFF-17)
#define TB_KEY_ARROW_UP		(0xFFFF-18)
#define TB_KEY_ARROW_DOWN	(0xFFFF-19)
#define TB_KEY_ARROW_LEFT	(0xFFFF-20)
#define TB_KEY_ARROW_RIGHT	(0xFFFF-21)

// These are all ASCII code points below SPACE character and a BACKSPACE key.
#define TB_KEY_CTRL_TILDE	0x00
#define TB_KEY_CTRL_2		0x00 // clash with 'CTRL_TILDE'
#define TB_KEY_CTRL_A		0x01
#define TB_KEY_CTRL_B		0x02
#define TB_KEY_CTRL_C		0x03
#define TB_KEY_CTRL_D		0x04
#define TB_KEY_CTRL_E		0x05
#define TB_KEY_CTRL_F		0x06
#define TB_KEY_CTRL_G		0x07
#define TB_KEY_BACKSPACE	0x08
#define TB_KEY_CTRL_H		0x08 // clash with 'CTRL_BACKSPACE'
#define TB_KEY_TAB		0x09
#define TB_KEY_CTRL_I		0x09 // clash with 'TAB'
#define TB_KEY_CTRL_J		0x0A
#define TB_KEY_CTRL_K		0x0B
#define TB_KEY_CTRL_L		0x0C
#define TB_KEY_ENTER		0x0D
#define TB_KEY_CTRL_M		0x0D // clash with 'ENTER'
#define TB_KEY_CTRL_N		0x0E
#define TB_KEY_CTRL_O		0x0F
#define TB_KEY_CTRL_P		0x10
#define TB_KEY_CTRL_Q		0x11
#define TB_KEY_CTRL_R		0x12
#define TB_KEY_CTRL_S		0x13
#define TB_KEY_CTRL_T		0x14
#define TB_KEY_CTRL_U		0x15
#define TB_KEY_CTRL_V		0x16
#define TB_KEY_CTRL_W		0x17
#define TB_KEY_CTRL_X		0x18
#define TB_KEY_CTRL_Y		0x19
#define TB_KEY_CTRL_Z		0x1A
#define TB_KEY_ESC		0x1B
#define TB_KEY_CTRL_LSQ_BRACKET	0x1B // clash with 'ESC'
#define TB_KEY_CTRL_3		0x1B // clash with 'ESC'
#define TB_KEY_CTRL_4		0x1C
#define TB_KEY_CTRL_BACKSLASH	0x1C // clash with 'CTRL_4'
#define TB_KEY_CTRL_5		0x1D
#define TB_KEY_CTRL_RSQ_BRACKET 0x1D // clash with 'CTRL_5'
#define TB_KEY_CTRL_6		0x1E
#define TB_KEY_CTRL_7		0x1F
#define TB_KEY_CTRL_SLASH	0x1F // clash with 'CTRL_7'
#define TB_KEY_CTRL_UNDERSCORE	0x1F // clash with 'CTRL_7'
#define TB_KEY_SPACE		0x20
#define TB_KEY_BACKSPACE2	0x7F
#define TB_KEY_CTRL_8		0x7F // clash with 'DELETE'

// These are non-existing ones.
//
// #define TB_KEY_CTRL_1 clash with '1'
// #define TB_KEY_CTRL_9 clash with '9'
// #define TB_KEY_CTRL_0 clash with '0'

// Currently there is only one modificator. See also struct tb_event's mod
// field.
#define TB_MOD_ALT	0x01

// Colors (see struct tb_cell's fg and bg fields).
#define TB_DEFAULT	0x00
#define TB_BLACK	0x01
#define TB_RED		0x02
#define TB_GREEN	0x03
#define TB_YELLOW	0x04
#define TB_BLUE		0x05
#define TB_MAGENTA	0x06
#define TB_CYAN		0x07
#define TB_WHITE	0x08

// Attributes, it is possible to use multiple attributes by combining them
// using bitwise OR ('|'). Although, colors cannot be combined. But you can
// combine attributes and a single color. See also struct tb_cell's fg and bg
// fields.
#define TB_BOLD		0x10
#define TB_UNDERLINE	0x20
#define TB_REVERSE	0x40

// A cell, single conceptual entity on the terminal screen. The terminal screen
// is basically a 2d array of cells. It has the following fields:
//  - 'ch' is a unicode character
//  - 'fg' foreground color and attributes
//  - 'bg' background color and attributes
struct tb_cell {
	uint32_t ch;
	uint16_t fg;
	uint16_t bg;
};

#define TB_EVENT_KEY 1
#define TB_EVENT_RESIZE 2

// This struct represents a termbox event. The 'mod', 'key' and 'ch' fields are
// valid if 'type' is TB_EVENT_KEY. The 'w' and 'h' fields are valid if 'type'
// is TB_EVENT_RESIZE. See also
struct tb_event {
	uint8_t type;
	uint8_t mod;
	uint16_t key;
	uint32_t ch;
	int32_t w;
	int32_t h;
};

// Error codes returned by tb_init(). All of them are self-explanatory, except
// the pipe trap error. Termbox uses unix pipes in order to deliever a message
// from a signal handler (SIGWINCH) to the main event reading loop. Honestly in
// most cases you should just check the returned code as < 0.
#define TB_EUNSUPPORTED_TERMINAL	-1
#define TB_EFAILED_TO_OPEN_TTY		-2
#define TB_EPIPE_TRAP_ERROR		-3

// Initializes the termbox library. This function should be called before any
// other functions. After successful initialization, the library must be
// finalized using the tb_shutdown() function.
SO_IMPORT int tb_init(void);
SO_IMPORT void tb_shutdown(void);

// Returns the size of the internal back buffer (which is the same as
// terminal's window size in characters). The internal buffer can be resized
// after tb_clear() or tb_present() function calls.
SO_IMPORT int tb_width(void);
SO_IMPORT int tb_height(void);

// Clears the internal back buffer using TB_DEFAULT color or the
// color/attributes set by tb_set_clear_attributes() function.
SO_IMPORT void tb_clear(void);
SO_IMPORT void tb_set_clear_attributes(uint16_t fg, uint16_t bg);

// Syncronizes the internal back buffer with the terminal.
SO_IMPORT void tb_present(void);

#define TB_HIDE_CURSOR -1

// Sets the position of the cursor. Upper-left character is (0, 0). If you pass
// TB_HIDE_CURSOR as both coordinates, then the cursor will be hidden. Cursor
// is hidden by default.
SO_IMPORT void tb_set_cursor(int cx, int cy);

// Changes cell's parameters in the internal back buffer at the specified
// position.
SO_IMPORT void tb_put_cell(int x, int y, const struct tb_cell *cell);
SO_IMPORT void tb_change_cell(int x, int y, uint32_t ch, uint16_t fg, uint16_t bg);

// Copies the buffer from 'cells' at the specified position, assuming the
// buffer is a two-dimensional array of size ('w' x 'h'), represented as a
// one-dimensional buffer containing lines of cells starting from the top.
SO_IMPORT void tb_blit(int x, int y, int w, int h, const struct tb_cell *cells);

#define TB_INPUT_CURRENT 0
#define TB_INPUT_ESC 1
#define TB_INPUT_ALT 2

// Sets the termbox input mode. Termbox has two input modes:
// 1. Esc input mode.
//    When ESC sequence is in the buffer and it doesn't match any known
//    ESC sequence => ESC means TB_KEY_ESC.
// 2. Alt input mode.
//    When ESC sequence is in the buffer and it doesn't match any known
//    sequence => ESC enables TB_MOD_ALT modifier for the next keyboard event.
//
// If 'mode' is TB_INPUT_CURRENT, it returns the current input mode.
SO_IMPORT int tb_select_input_mode(int mode);


// Wait for an event up to 'timeout' milliseconds and fill the 'event'
// structure with it, when the event is available. Returns the type of the
// event (one of TB_EVENT_* constants) or -1 if there was an error or 0 in case
// there were no event during 'timeout' period.
SO_IMPORT int tb_peek_event(struct tb_event *event, int timeout);

// Wait for an event forever and fill the 'event' structure with it, when the
// event is available. Returns the type of the event (one of TB_EVENT_*
// constants) or -1 if there was an error.
SO_IMPORT int tb_poll_event(struct tb_event *event);

// Utility utf8 functions.
#define TB_EOF -1
SO_IMPORT int tb_utf8_char_length(char c);
SO_IMPORT int tb_utf8_char_to_unicode(uint32_t *out, const char *c);
SO_IMPORT int tb_utf8_unicode_to_char(char *out, uint32_t c);

#ifdef __cplusplus
}
#endif
