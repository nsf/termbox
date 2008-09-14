#!/usr/bin/python

import sys, os, curses

def puttc(str):
	if str == None:
		return
	for c in str:
		if ord(c) == 0x1B:
			sys.stdout.write("\\033")
		else:
			sys.stdout.write(c)

keys = [
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
	"INSERT",
	"DELETE",
	"HOME",
	"END",
	"PGUP",
	"PGDN",
	"KEY_UP",
	"KEY_DOWN",
	"KEY_LEFT",
	"KEY_RIGHT"
]

funcs = [
	"T_ENTER_CA", "smcup",
	"T_EXIT_CA", "rmcup",
	"T_SHOW_CURSOR", "cnorm",
	"T_HIDE_CURSOR", "civis",
	"T_CLEAR_SCREEN", "clear",
	"T_SGR", "sgr",
	"T_SGR0", "sgr0",
	"T_UNDERLINE", "smul",
	"T_BOLD", "bold",
	"T_BLINK", "blink",
	"T_MOVE_CURSOR", "cup"
]

def pairsiter(iterable):
	iterable = iter(iterable)
	while True:
		yield (iterable.next(), iterable.next())

answers = []

for i in keys:
	answers.append(raw_input("Press %s and Enter: " % i))

print "Collecting done, result:"
print "----------------cut_and_paste_it_to_nsf-------------------"

sys.stdout.write("const char *%s_keys[] = {\n\t" % os.getenv("TERM"))
for i in answers:
	sys.stdout.write("\"")
	puttc(i)
	sys.stdout.write("\",")
sys.stdout.write(" 0\n};\n")
curses.setupterm()
sys.stdout.write("const char *%s_funcs[] = {\n" % os.getenv("TERM"))
for k,v in pairsiter(funcs):
	sys.stdout.write("\t[%s] = \"" % k)
	if v == "sgr":
		sys.stdout.write("\\033[3%u;4%um")
	elif v == "cup":
		sys.stdout.write("\\033[%u;%uH")
	else:
		tc = curses.tigetstr(v)
		puttc(tc)
	if v == "cup":
		sys.stdout.write("\"\n")
	else:
		sys.stdout.write("\",\n")
sys.stdout.write("};\n")
print "----------------------------------------------------------"

