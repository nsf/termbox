#!/usr/bin/python

import curses, sys, os

def print_cap(capname, cap):
	capstr = curses.tigetstr(cap)
	sys.stdout.write("[%s] = \"" % capname)
	if capstr is not None:
		for c in capstr:
			if ord(c) < 32:
				sys.stdout.write("\\0%o" % ord(c))
			else:
				sys.stdout.write(c)
	sys.stdout.write("\",\n")
	sys.stdout.flush()

notok = False

def check_cap(capname, cap, prn):
	global notok
	capstr = curses.tigetstr(cap)
	if capstr is None:
		print "%s \033[31mFAIL\033[0m (%s)" % (os.getenv("TERM"), capname)
		notok = True
	if prn == "print":
		print_cap(capname, cap)


try: curses.setupterm()
except: sys.exit(1)

if len(sys.argv) > 1:
	prn = sys.argv[1]
else:
	prn = None

check_cap('ENTER_CA', 'smcup', prn)
check_cap('EXIT_CA', 'rmcup', prn)
check_cap('SHOW_CURSOR', 'cnorm', prn)
check_cap('HIDE_CURSOR', 'civis', prn)
check_cap('CLEAR_SCREEN', 'clear', prn)
check_cap('SGR', 'sgr', prn)
check_cap('SGR0', 'sgr0', prn)
check_cap('UNDERLINE', 'smul', prn)
check_cap('BOLD', 'bold', prn)
check_cap('BLINK', 'blink', prn)
check_cap('MOVE_CURSOR', 'cup', prn)
check_cap('ENTER_KEYPAD', 'smkx', prn)
check_cap('EXIT_KEYPAD', 'rmkx', prn)

check_cap('F1', 'kf1', prn)
check_cap('F2', 'kf2', prn)
check_cap('F3', 'kf3', prn)
check_cap('F4', 'kf4', prn)
check_cap('F5', 'kf5', prn)
check_cap('F6', 'kf6', prn)
check_cap('F7', 'kf7', prn)
check_cap('F8', 'kf8', prn)
check_cap('F9', 'kf9', prn)
check_cap('F10', 'kf10', prn)
check_cap('F11', 'kf11', prn)
check_cap('F12', 'kf12', prn)

check_cap('INSERT', 'kich1', prn)
check_cap('DELETE', 'kdch1', prn)
check_cap('HOME', 'khome', prn)
check_cap('END', 'kend', prn)
check_cap('PGUP', 'kpp', prn)
check_cap('PGDN', 'knp', prn)

check_cap('KEY_UP', 'kcuu1', prn)
check_cap('KEY_DOWN', 'kcud1', prn)
check_cap('KEY_LEFT', 'kcub1', prn)
check_cap('KEY_RIGHT', 'kcuf1', prn)

#check_cap('SHIFT_TAB', 'kcbt', prn)
#check_cap('SHIFT_F1', 'kf11', prn)
#check_cap('SHIFT_F2', 'kf12', prn)
#check_cap('SHIFT_F3', 'kf13', prn)
#check_cap('SHIFT_F4', 'kf14', prn)
#check_cap('SHIFT_F5', 'kf15', prn)
#check_cap('SHIFT_F6', 'kf16', prn)
#check_cap('SHIFT_F7', 'kf17', prn)
#check_cap('SHIFT_F8', 'kf18', prn)
#check_cap('SHIFT_F9', 'kf19', prn)
#check_cap('SHIFT_F10', 'kf20', prn)

if not notok:
	print "%s \033[32mOK\033[0m" % os.getenv("TERM")
