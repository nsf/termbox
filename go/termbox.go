// Termbox library provides facilities for terminal input/output manipulation
// in a pseudo-GUI style.
package termbox

// #include "../termbox.h"
import "C"
import (
	"unsafe"
	"os"
)

// This type represents termbox event. 'Mod', 'Key' and 'Ch' fields are valid
// if 'Type' is EVENT_KEY. 'W' and 'H' are valid if 'Type' is EVENT_RESIZE.
type Event struct {
	Type uint8  // one of EVENT_ constants
	Mod  uint8  // one of MOD_ constants or 0
	Key  uint16 // one of Key constants, invalid if 'Ch' is not 0
	Ch   uint32 // a unicode character
	W    int32  // width of the screen
	H    int32  // height of the screen
}

// A cell, single conceptual entity on the screen. The screen is basically a 2d
// array of cells. 'Ch' is a unicode character, 'Fg' and 'Bg' are foreground
// and background attributes respectively.
type Cell struct {
	Ch uint32
	Fg uint16
	Bg uint16
}

type struct_tb_event_ptr *C.struct_tb_event
type struct_tb_cell_ptr *C.struct_tb_cell

// Key constants, see Event.Key field.
const (
	KeyF1 = (0xFFFF - iota)
	KeyF2
	KeyF3
	KeyF4
	KeyF5
	KeyF6
	KeyF7
	KeyF8
	KeyF9
	KeyF10
	KeyF11
	KeyF12
	KeyInsert
	KeyDelete
	KeyHome
	KeyEnd
	KeyPgUp
	KeyPgDn
	KeyArrowUp
	KeyArrowDown
	KeyArrowLeft
	KeyArrowRight
)

const (
	KeyCtrlTilde = iota
	KeyCtrlA
	KeyCtrlB
	KeyCtrlC
	KeyCtrlD
	KeyCtrlE
	KeyCtrlF
	KeyCtrlG
	KeyBackspace
	KeyTab
	KeyCtrlJ
	KeyCtrlK
	KeyCtrlL
	KeyEnter
	KeyCtrlN
	KeyCtrlO
	KeyCtrlP
	KeyCtrlQ
	KeyCtrlR
	KeyCtrlS
	KeyCtrlT
	KeyCtrlU
	KeyCtrlV
	KeyCtrlW
	KeyCtrlX
	KeyCtrlY
	KeyCtrlZ
	KeyEsc
	KeyCtrl4
	KeyCtrl5
	KeyCtrl6
	KeyCtrl7
	KeySPACE
)

const (
	KeyCtrl2          = KeyCtrlTilde
	KeyCtrlH          = KeyBackspace
	KeyCtrlI          = KeyTab
	KeyCtrlM          = KeyEnter
	KeyCtrlLSQBracket = KeyEsc
	KeyCtrl3          = KeyEsc
	KeyCtrlBackslash  = KeyCtrl4
	KeyCtrlRSQBracket = KeyCtrl5
	KeyCtrlSlash      = KeyCtrl7
	KeyCtrlUnderscore = KeyCtrl7
	KeyBackspace2     = 0x7F
	KeyCtrl8          = 0x7F
)

// Alt modifier constant, see Event.Mod field and SetInputMode function.
const MOD_ALT = 0x01

// Cell attributes, it is possible to use multiple attributes by combining them
// using bitwise OR ('|'). Although, colors cannot be combined. But you can
// combine attributes and a single color.
const (
	Black = iota
	Red
	Green
	Yellow
	Blue
	Magenta
	Cyan
	White
)

const (
	Bold      = 0x10
	Underline = 0x20
)

// Special coordinate for SetCursor. If you call:
//	SetCursor(CHideCursor, CHideCursor)
// This function call hides the cursor.
const CHideCursor = -1

// Input mode. See SelectInputMode function.
const (
	_ = iota
	InputEsc
	InputAlt
)

// Event type. See Event.Type field.
const (
	_ = iota
	EventKey
	EventResize
)

// Initializes termbox library. This function should be called before any other functions.
// After successful initialization, the library must be finalized using 'Shutdown' function.
//
// Example usage:
//	err := termbox.Init()
//	if err != nil {
//		panic(err.String())
//	}
//	defer termbox.Shutdown()
func Init() os.Error {
	switch int(C.tb_init()) {
	case -3:
		return os.NewError("Pipe trap error")
	case -2:
		return os.NewError("Failed to open /dev/tty")
	case -1:
		return os.NewError("Unsupported terminal")
	}
	return nil
}

// Finalizes termbox library, should be called after successful initialization
// when termbox's functionality isn't required anymore.
func Shutdown() { C.tb_shutdown() }

// Changes cell's parameters in the internal back buffer at the specified
// position.
func ChangeCell(x int, y int, ch int, fg uint16, bg uint16) {
	C.tb_change_cell(C.uint(x), C.uint(y), C.uint32_t(ch), C.uint16_t(fg), C.uint16_t(bg))
}

// Puts the 'cell' into the internal back buffer at the specified position.
func PutCell(x, y int, cell *Cell) {
	C.tb_put_cell(C.uint(x), C.uint(y), struct_tb_cell_ptr(unsafe.Pointer(cell)))
}

// 'Blit' function copies the 'cells' buffer to the internal back buffer at the
// position specified by 'x' and 'y'. Blit doesn't perform any kind of cuts and
// if contents of the cells buffer cannot be placed without crossing back
// buffer's boundaries, the operation is discarded. Parameter 'w' must be > 0,
// otherwise it will cause "division by zero" panic.
//
// The width and the height of the 'cells' buffer are calculated that way:
//	w := w
//	h := len(cells) / w
func Blit(x, y, w int, cells []Cell) {
	h := len(cells) / w
	C.tb_blit(C.uint(x), C.uint(y), C.uint(w), C.uint(h), struct_tb_cell_ptr(unsafe.Pointer(&cells[0])))
}

// Synchronizes the internal back buffer with the terminal.
func Present() { C.tb_present() }

// Clears the internal back buffer.
func Clear() { C.tb_clear() }

// Wait for an event. This is a blocking function call. If an error occurs,
// returns -1. Otherwise the return value is one of EVENT_ consts.
func PollEvent(e *Event) int {
	return int(C.tb_poll_event(struct_tb_event_ptr(unsafe.Pointer(e))))
}

// Wait for an event 'timeout' milliseconds. If no event occurs, returns 0. If
// an error occurs, returns -1. Otherwise the return value is one of EVENT_
// consts.
func PeekEvent(e *Event, timeout int) int {
	return int(C.tb_peek_event(struct_tb_event_ptr(unsafe.Pointer(e)), C.uint(timeout)))
}

// Returns the width of the internal back buffer (which is the same as
// terminal's window width in characters).
func Width() int { return int(C.tb_width()) }

// Returns the height of the internal back buffer (which is the same as
// terminal's window height in characters).
func Height() int { return int(C.tb_height()) }

// Sets the position of the cursor. See also HIDE_CURSOR and HideCursor().
func SetCursor(x int, y int) { C.tb_set_cursor(C.int(x), C.int(y)) }

// The shortcut for SetCursor(HIDE_CURSOR, HIDE_CURSOR).
func HideCursor() { C.tb_set_cursor(CHideCursor, CHideCursor) }

// Selects termbox input mode. Termbox has two input modes:
//
// 1. ESC input mode. When ESC sequence is in the buffer and it doesn't
// match any known sequence. ESC means KeyESC.
//
// 2. ALT input mode. When ESC sequence is in the buffer and it doesn't match
// any known sequence. ESC enables MOD_ALT modifier for the next keyboard
// event.
//
// If 'mode' is 0, returns the current input mode. See also INPUT_ constants.
//
// Note: INPUT_ALT mode may not work with PeekEvent.
func SelectInputMode(mode int) { C.tb_select_input_mode(C.int(mode)) }

// Shortcut for termbox.PollEvent(e).
func (e *Event) Poll() int { return PollEvent(e) }

// Shortcut for termbox.PeekEvent(e, timeout).
func (e *Event) Peek(timeout int) int { return PeekEvent(e, timeout) }
