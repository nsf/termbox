#!/usr/bin/python
# -*- encoding: utf-8 -*-

import termbox
import time
import sys
import random
#import psyco

#psyco.full()

spaceord = ord(u" ")

def print_line(t, msg, y, fg, bg):
	w = t.width()
	l = len(msg)
	x = 0
	for i in range(w):
		c = spaceord
		if i < l:
			c = ord(msg[i])
		t.change_cell(x+i, y, c, fg, bg)

class SelectBox(object):
	def __init__(self, tb, choices, active=-1):
		self.tb = tb
		self.active = active
		self.choices = choices
		self.color_active = (termbox.BLACK, termbox.CYAN)
		self.color_normal = (termbox.WHITE, termbox.BLACK)

	def draw(self):
		for i, c in enumerate(self.choices):
			color = self.color_normal
			if i == self.active:
				color = self.color_active
			print_line(self.tb, c, i, *color)

	def validate_active(self):
		if self.active < 0:
			self.active = 0
		if self.active >= len(self.choices):
			self.active = len(self.choices)-1

	def set_active(self, i):
		self.active = i
		self.validate_active()

	def move_up(self):
		self.active -= 1
		self.validate_active()

	def move_down(self):
		self.active += 1
		self.validate_active()

choices = [
	u"This instructs Psyco",
	u"to compile and run as",
	u"much of your application",
	u"code as possible. This is the",
	u"simplest interface to Psyco.",
	u"In good cases you can just add",
	u"these two lines and enjoy the speed-up.",
	u"If your application does a lot",
	u"of initialization stuff before",
	u"the real work begins, you can put",
	u"the above two lines after this",
	u"initialization - e.g. after importing",
	u"modules, creating constant global objects, etc.",
	u"This instructs Psyco",
	u"to compile and run as",
	u"much of your application",
	u"code as possible. This is the",
	u"simplest interface to Psyco.",
	u"In good cases you can just add",
	u"these two lines and enjoy the speed-up.",
	u"If your application does a lot",
	u"of initialization stuff before",
	u"the real work begins, you can put",
	u"the above two lines after this",
	u"initialization - e.g. after importing",
	u"modules, creating constant global objects, etc."
]

def draw_bottom_line(t, i):
	i = i % 8
	w = t.width()
	h = t.height()
	c = i
	palette = [termbox.DEFAULT, termbox.BLACK, termbox.RED, termbox.GREEN,
	           termbox.YELLOW, termbox.BLUE, termbox.MAGENTA, termbox.CYAN,
	           termbox.WHITE]
	for x in range(w):
		t.change_cell(x, h-1, ord(u' '), termbox.BLACK, palette[c])
		t.change_cell(x, h-2, ord(u' '), termbox.BLACK, palette[c])
		c += 1
		if c > 7:
			c = 0

with termbox.Termbox() as t:
	sb = SelectBox(t, choices, 0)
	t.clear()
	sb.draw()
	t.present()
	i = 0
	run_app = True
	while run_app:
		event_here = t.poll_event()
		while event_here:
			(type, ch, key, mod, w, h, x, y) = event_here
			if type == termbox.EVENT_KEY and key == termbox.KEY_ESC:
				run_app = False
			if type == termbox.EVENT_KEY:
				if key == termbox.KEY_ARROW_DOWN:
					sb.move_down()
				elif key == termbox.KEY_ARROW_UP:
					sb.move_up()
				elif key == termbox.KEY_HOME:
					sb.set_active(-1)
				elif key == termbox.KEY_END:
					sb.set_active(999)
			event_here = t.peek_event()

		t.clear()
		sb.draw()
		draw_bottom_line(t, i)
		t.present()
		i += 1
