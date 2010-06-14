package termbox

// #include "../termbox.h"
import "C"
import "unsafe"

type struct_tb_event_ptr *_Cstruct_tb_event

const (
	KEY_F1			= (0xFFFF-0)
	KEY_F2			= (0xFFFF-1)
	KEY_F3			= (0xFFFF-2)
	KEY_F4			= (0xFFFF-3)
	KEY_F5			= (0xFFFF-4)
	KEY_F6			= (0xFFFF-5)
	KEY_F7			= (0xFFFF-6)
	KEY_F8			= (0xFFFF-7)
	KEY_F9			= (0xFFFF-8)
	KEY_F10			= (0xFFFF-9)
	KEY_F11			= (0xFFFF-10)
	KEY_F12			= (0xFFFF-11)
	KEY_INSERT		= (0xFFFF-12)
	KEY_DELETE		= (0xFFFF-13)
	KEY_HOME		= (0xFFFF-14)
	KEY_END			= (0xFFFF-15)
	KEY_PGUP		= (0xFFFF-16)
	KEY_PGDN		= (0xFFFF-17)
	KEY_ARROW_UP		= (0xFFFF-18)
	KEY_ARROW_DOWN		= (0xFFFF-19)
	KEY_ARROW_LEFT		= (0xFFFF-20)
	KEY_ARROW_RIGHT		= (0xFFFF-21)

	KEY_CTRL_TILDE		= 0x00
	KEY_CTRL_2		= 0x00
	KEY_CTRL_A		= 0x01
	KEY_CTRL_B		= 0x02
	KEY_CTRL_C		= 0x03
	KEY_CTRL_D		= 0x04
	KEY_CTRL_E		= 0x05
	KEY_CTRL_F		= 0x06
	KEY_CTRL_G		= 0x07
	KEY_BACKSPACE		= 0x08
	KEY_CTRL_H		= 0x08
	KEY_TAB			= 0x09
	KEY_CTRL_I		= 0x09
	KEY_CTRL_J		= 0x0A
	KEY_CTRL_K		= 0x0B
	KEY_CTRL_L		= 0x0C
	KEY_ENTER		= 0x0D
	KEY_CTRL_M		= 0x0D
	KEY_CTRL_N		= 0x0E
	KEY_CTRL_O		= 0x0F
	KEY_CTRL_P		= 0x10
	KEY_CTRL_Q		= 0x11
	KEY_CTRL_R		= 0x12
	KEY_CTRL_S		= 0x13
	KEY_CTRL_T		= 0x14
	KEY_CTRL_U		= 0x15
	KEY_CTRL_V		= 0x16
	KEY_CTRL_W		= 0x17
	KEY_CTRL_X		= 0x18
	KEY_CTRL_Y		= 0x19
	KEY_CTRL_Z		= 0x1A
	KEY_ESC			= 0x1B
	KEY_CTRL_LSQ_BRACKET	= 0x1B
	KEY_CTRL_3		= 0x1B
	KEY_CTRL_4		= 0x1C
	KEY_CTRL_BACKSLASH	= 0x1C
	KEY_CTRL_5		= 0x1D
	KEY_CTRL_RSQ_BRACKET	= 0x1D
	KEY_CTRL_6		= 0x1E
	KEY_CTRL_7		= 0x1F
	KEY_CTRL_SLASH		= 0x1F
	KEY_CTRL_UNDERSCORE	= 0x1F
	KEY_SPACE		= 0x20
	KEY_BACKSPACE2		= 0x7F
	KEY_CTRL_8		= 0x7F

	MOD_ALT			= 0x01

	BLACK			= 0x00
	RED			= 0x01
	GREEN			= 0x02
	YELLOW			= 0x03
	BLUE			= 0x04
	MAGENTA			= 0x05
	CYAN			= 0x06
	WHITE			= 0x07

	BOLD			= 0x10
	UNDERLINE		= 0x20

	HIDE_CURSOR		= -1
	INPUT_ESC		= 1
	INPUT_ALT		= 2
	EVENT_KEY		= 1
	EVENT_RESIZE		= 2
)

func Init() int {
	return int(C.tb_init());
}

func Shutdown() {
	C.tb_shutdown();
}

func ChangeCell(x uint, y uint, ch int, fg uint16, bg uint16) {
	C.tb_change_cell(C.uint(x), C.uint(y), C.uint32_t(ch), C.uint16_t(fg), C.uint16_t(bg));
}

func Present() {
	C.tb_present();
}

func Clear() {
	C.tb_clear();
}

func PollEvent(e *Event) int {
	return int(C.tb_poll_event(struct_tb_event_ptr(unsafe.Pointer(e))))
}

func PeekEvent(e *Event, timeout uint) int {
	return int(C.tb_peek_event(struct_tb_event_ptr(unsafe.Pointer(e)), C.uint(timeout)))
}

func Width() uint {
	return uint(C.tb_width())
}

func Height() uint {
	return uint(C.tb_height())
}

func SetCursor(x int, y int) {
	C.tb_set_cursor(C.int(x), C.int(y))
}

func HideCursor() {
	C.tb_set_cursor(-1, -1)
}

func SelectInputMode(mode int) {
	C.tb_select_input_mode(C.int(mode))
}

func (e *Event) Poll() int {
	return PollEvent(e)
}

func (e *Event) Peek(timeout uint) int {
	return PeekEvent(e, timeout)
}
