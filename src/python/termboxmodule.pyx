"""Termbox library python binding.

This is a binding module for termbox library.
"""

include "termboxconfig.pyx"

## No unichr in python3, use chr instead.
if PY_MAJOR_VERSION == 3:
	unichr = chr

cdef extern from "stdint.h":
	ctypedef unsigned int uint32_t
	ctypedef unsigned short uint16_t
	ctypedef signed int int32_t

cdef extern from "../termbox.h":
	struct tb_event:
		uint16_t type
		uint32_t ch
		uint16_t key
		uint16_t mod
		int32_t w
		int32_t h
		int32_t x
		int32_t y
	int tb_init()
	void tb_shutdown()
	void tb_present()
	void tb_clear()
	void tb_change_cell(int x, int y, uint32_t ch, uint16_t fg, uint16_t bg)
	int tb_width()
	int tb_height()
	void tb_set_cursor(int x, int y)
	int tb_select_input_mode(int mode)
	int tb_select_output_mode(int mode)
	int tb_peek_event(tb_event *event, int timeout) nogil
	int tb_poll_event(tb_event *event) nogil

class TermboxException(Exception):
	def __init__(self, msg):
		self.msg = msg
	def __str__(self):
		return self.msg

import threading

__instance = None

# keys ----------------------------------
KEY_F1               = (0xFFFF-0)
KEY_F2               = (0xFFFF-1)
KEY_F3               = (0xFFFF-2)
KEY_F4               = (0xFFFF-3)
KEY_F5               = (0xFFFF-4)
KEY_F6               = (0xFFFF-5)
KEY_F7               = (0xFFFF-6)
KEY_F8               = (0xFFFF-7)
KEY_F9               = (0xFFFF-8)
KEY_F10              = (0xFFFF-9)
KEY_F11              = (0xFFFF-10)
KEY_F12              = (0xFFFF-11)
KEY_INSERT           = (0xFFFF-12)
KEY_DELETE           = (0xFFFF-13)
KEY_HOME             = (0xFFFF-14)
KEY_END              = (0xFFFF-15)
KEY_PGUP             = (0xFFFF-16)
KEY_PGDN             = (0xFFFF-17)
KEY_ARROW_UP         = (0xFFFF-18)
KEY_ARROW_DOWN       = (0xFFFF-19)
KEY_ARROW_LEFT       = (0xFFFF-20)
KEY_ARROW_RIGHT      = (0xFFFF-21)
KEY_MOUSE_LEFT      =(0xFFFF-22)
KEY_MOUSE_RIGHT      =(0xFFFF-23)
KEY_MOUSE_MIDDLE      =(0xFFFF-24)
KEY_MOUSE_RELEASE      =(0xFFFF-25)
KEY_MOUSE_WHEEL_UP      =(0xFFFF-26)
KEY_MOUSE_WHEEL_DOWN      =(0xFFFF-27)

KEY_CTRL_TILDE       = 0x00
KEY_CTRL_2           = 0x00
KEY_CTRL_A           = 0x01
KEY_CTRL_B           = 0x02
KEY_CTRL_C           = 0x03
KEY_CTRL_D           = 0x04
KEY_CTRL_E           = 0x05
KEY_CTRL_F           = 0x06
KEY_CTRL_G           = 0x07
KEY_BACKSPACE        = 0x08
KEY_CTRL_H           = 0x08
KEY_TAB              = 0x09
KEY_CTRL_I           = 0x09
KEY_CTRL_J           = 0x0A
KEY_CTRL_K           = 0x0B
KEY_CTRL_L           = 0x0C
KEY_ENTER            = 0x0D
KEY_CTRL_M           = 0x0D
KEY_CTRL_N           = 0x0E
KEY_CTRL_O           = 0x0F
KEY_CTRL_P           = 0x10
KEY_CTRL_Q           = 0x11
KEY_CTRL_R           = 0x12
KEY_CTRL_S           = 0x13
KEY_CTRL_T           = 0x14
KEY_CTRL_U           = 0x15
KEY_CTRL_V           = 0x16
KEY_CTRL_W           = 0x17
KEY_CTRL_X           = 0x18
KEY_CTRL_Y           = 0x19
KEY_CTRL_Z           = 0x1A
KEY_ESC              = 0x1B
KEY_CTRL_LSQ_BRACKET = 0x1B
KEY_CTRL_3           = 0x1B
KEY_CTRL_4           = 0x1C
KEY_CTRL_BACKSLASH   = 0x1C
KEY_CTRL_5           = 0x1D
KEY_CTRL_RSQ_BRACKET = 0x1D
KEY_CTRL_6           = 0x1E
KEY_CTRL_7           = 0x1F
KEY_CTRL_SLASH       = 0x1F
KEY_CTRL_UNDERSCORE  = 0x1F
KEY_SPACE            = 0x20
KEY_BACKSPACE2       = 0x7F
KEY_CTRL_8           = 0x7F

MOD_ALT              = 0x01

# attributes ----------------------

DEFAULT   = 0x00
BLACK     = 0x01
RED       = 0x02
GREEN     = 0x03
YELLOW    = 0x04
BLUE      = 0x05
MAGENTA   = 0x06
CYAN      = 0x07
WHITE     = 0x08

BOLD      = 0x0100
UNDERLINE = 0x0200
REVERSE   = 0x0400

# misc ----------------------------

HIDE_CURSOR      = -1
INPUT_CURRENT    = 0
INPUT_ESC        = 1
INPUT_ALT        = 2
OUTPUT_CURRENT   = 0
OUTPUT_NORMAL    = 1
OUTPUT_256       = 2
OUTPUT_216       = 3
OUTPUT_GRAYSCALE = 4
EVENT_KEY        = 1
EVENT_RESIZE     = 2
EVENT_MOUSE		= 3

cdef class Termbox:
	cdef int created
	cdef object _poll_lock

	def __cinit__(self):
		cdef int ret
		global __instance

		self.created = 0
		self._poll_lock = threading.Lock()

		if __instance:
			raise TermboxException("It is possible to create only one instance of Termbox")

		ret = tb_init()
		ret = 1
		if ret < 0:
			raise TermboxException("Failed to init Termbox")
		__instance = self
		self.created = 1

	def __dealloc__(self):
		self.close()

	def __del__(self):
		self.close()

	def __exit__(self, *args):#t, value, traceback):
		self.close()

	def __enter__(self):
		return self

	def close(self):
		global __instance
		if self.created:
			tb_shutdown()
			self.created = 0
			__instance = None

	def present(self):
		"""Sync state of the internal cell buffer with the terminal.
		"""
		tb_present()
		pass

	def change_cell(self, int x, int y, int ch, uint16_t fg, uint16_t bg):
		"""Change cell in position (x;y).
		"""
		tb_change_cell(x, y, ch, fg, bg)

	def width(self):
		"""Returns width of the terminal screen.
		"""
		return int(tb_width())

	def height(self):
		"""Return height of the terminal screen.
		"""
		return int(tb_height())

	def clear(self):
		"""Clear the internal cell buffer.
		"""
		tb_clear()

	def set_cursor(self, int x, int y):
		"""Set cursor position to (x;y).

		   Set both arguments to HIDE_CURSOR or use 'hide_cursor' function to hide it.
		"""
		tb_set_cursor(x, y)

	def hide_cursor(self):
		"""Hide cursor.
		"""
		tb_set_cursor(-1, -1)

	def select_input_mode(self, int mode):
		"""Select preferred input mode: INPUT_ESC or INPUT_ALT.

		   INPUT_CURRENT returns the selected mode without changing anything.
		"""
		return int(tb_select_input_mode(mode))

	def select_output_mode(self, int mode):
		"""Select preferred output mode: one of OUTPUT_* constants.

		   OUTPUT_CURRENT returns the selected mode without changing anything.
		"""
		return int(tb_select_output_mode(mode))

	def peek_event(self, int timeout=0):
		"""Wait for an event up to 'timeout' milliseconds and return it.

		   Returns None if there was no event and timeout is expired.
		   Returns a tuple otherwise: (type, unicode character, key, mod, width, height, mousex, mousey).
		"""
		cdef tb_event e
		cdef int result
		with self._poll_lock:
			with nogil:
				result = tb_peek_event(&e, timeout)
		assert(result >= 0)
		if result == 0:
			return None
		if e.ch:
			uch = unichr(e.ch)
		else:
			uch = None
		return (e.type, uch, e.key, e.mod, e.w, e.h, e.x, e.y)

	def poll_event(self):
		"""Wait for an event and return it.

		   Returns a tuple: (type, unicode character, key, mod, width, height, mousex, mousey).
		"""
		cdef tb_event e
		cdef int result
		with self._poll_lock:
			with nogil:
				result = tb_poll_event(&e)
		assert(result >= 0)
		if e.ch:
			uch = unichr(e.ch)
		else:
			uch = None
		return (e.type, uch, e.key, e.mod, e.w, e.h, e.x, e.y)
