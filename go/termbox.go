// Termbox library provides facilities for terminal input/output manipulation
// in a pseudo-GUI style.
package termbox

// #include "../termbox.h"
import "C"
import (
	"os"
	"fmt"
	"utf8"
	"unsafe"
)

// This type represents termbox event. 'Mod', 'Key' and 'Ch' fields are valid
// if 'Type' is EventKey. 'W' and 'H' are valid if 'Type' is EventResize.
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
const ModAlt = 0x01

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
//
//     SetCursor(Hide_Cursor, Hide_Cursor)
//
// This function call hides the cursor.
const Hide_Cursor = -1

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

// Init initializes the termbox library. This function should be called before
// any other functions. After successful initialization, the library must be
// finalized using 'Shutdown' function.
//
// Example usage:
//	err := termbox.Init()
//	if err != nil {
//		panic(err.String())
//	}
//	defer termbox.Shutdown()
func Init() (err os.Error) {
	switch int(C.tb_init()) {
	case -3:
		err = os.NewError("Pipe trap error")
	case -2:
		err = os.NewError("Failed to open /dev/tty")
	case -1:
		err = os.NewError("Unsupported terminal")
	}
	return
}

// Shutdown finalizes the termbox library, should be called after successful
// initialization when termbox's functionality isn't required anymore.
func Shutdown() { C.tb_shutdown() }

// ChangeCell alters the back buffer cell parameters at the specified position.
func ChangeCell(x, y, ch int, fg, bg uint16) {
	C.tb_change_cell(C.uint(x), C.uint(y), C.uint32_t(ch), C.uint16_t(fg), C.uint16_t(bg))
}

// PutCell puts the cell at the specified position into the back buffer.
func PutCell(x, y int, cell *Cell) {
	C.tb_put_cell(C.uint(x), C.uint(y), struct_tb_cell_ptr(unsafe.Pointer(cell)))
}

// A convenience function for printing the given string starting at the
// given cell.
func Printf(x, y int, fg, bg uint16, format string, argv ...interface{}) {
	if format = fmt.Sprintf(format, argv...); len(format) == 0 {
		return
	}

	cfg := C.uint16_t(fg)
	cbg := C.uint16_t(bg)
	cx := C.uint(x)
	cy := C.uint(y)

	var rune, size int
	for len(format) > 0 {
		if rune, size = utf8.DecodeRuneInString(format); size == 0 {
			break
		}

		format = format[size:]
		C.tb_change_cell(cx, cy, C.uint32_t(rune), cfg, cbg)
		cx++
	}
}

// Blit copies the cells buffer to the internal back buffer at the position
// specified by 'x' and 'y'. Blit doesn't perform any kind of cuts and if
// contents of the cells buffer cannot be placed without crossing back
// buffer's boundaries, the operation is discarded.
//
// The height of the 'cells' buffer is calculated as follows:
//
//     h := len(cells) / w
func Blit(x, y, w int, cells []Cell) {
	if w <= 0 {
		return
	}

	h := len(cells) / w
	C.tb_blit(C.uint(x), C.uint(y), C.uint(w), C.uint(h),
		struct_tb_cell_ptr(unsafe.Pointer(&cells[0])))
}

// Present synchronizes the internal back buffer with the terminal.
func Present() { C.tb_present() }

// Clear clears the internal back buffer.
func Clear() { C.tb_clear() }

// PollEvent waits for an event. This is a blocking function call. If an error
// occurs, it returns -1. Otherwise the return value is one of EventXXX
// constants.
func PollEvent(e *Event) int {
	return int(C.tb_poll_event(struct_tb_event_ptr(unsafe.Pointer(e))))
}

// PeekEvent waits for an event for 'timeout' milliseconds. If no event occurs,
// it returns 0. If an error occurs, it returns -1. Otherwise the return value
// is one of EventXXX constants.
func PeekEvent(e *Event, timeout int) int {
	return int(C.tb_peek_event(struct_tb_event_ptr(unsafe.Pointer(e)), C.uint(timeout)))
}

// Width returns the width of the internal back buffer (which is the same as
// terminal's window width in characters).
func Width() int { return int(C.tb_width()) }

// Height returns the height of the internal back buffer (which is the same as
// terminal's window height in characters).
func Height() int { return int(C.tb_height()) }

// SetCursor sets the position of the cursor. See also Hide_Cursor and HideCursor().
func SetCursor(x int, y int) { C.tb_set_cursor(C.int(x), C.int(y)) }

// HideCursor is a shortcut for SetCursor(Hide_Cursor, Hide_Cursor).
func HideCursor() { C.tb_set_cursor(Hide_Cursor, Hide_Cursor) }

// SelectInputMode selects the termbox input mode. Termbox has two input modes:
//
// 1. ESC input mode. When an ESC sequence is in the buffer and it doesn't
// match any known sequence. ESC means KeyEsc.
//
// 2. ALT input mode. When an ESC sequence is in the buffer and it doesn't match
// any known sequence. ESC enables the ModAlt modifier for the next keyboard
// event.
//
// If 'mode' is 0, returns the current input mode. Refer to the InputXXX
// constants for details.
//
// Note: InputAlt mode may not work with PeekEvent.
func SelectInputMode(mode int) { C.tb_select_input_mode(C.int(mode)) }

// Poll is a shortcut for PollEvent(e).
func (e *Event) Poll() int { return PollEvent(e) }

// Peek is a shortcut for PeekEvent(e, timeout).
func (e *Event) Peek(timeout int) int { return PeekEvent(e, timeout) }
