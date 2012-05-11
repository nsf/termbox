#!/usr/bin/python

import sys, os, curses

def puttc(str):
	if str == None:
		return
	for c in str:
		if c == 0x1B:
			sys.stdout.write("\\033")
		else:
			sys.stdout.write(chr(c))

terminals = {
	'xterm' : 'xterm',
	'rxvt-256color' : 'rxvt_256color',
	'rxvt-unicode' : 'rxvt_unicode',
	'linux' : 'linux',
	'Eterm' : 'Eterm',
	'screen' : 'screen'
}

keys = [
	"F1",		"kf1",
	"F2",		"kf2",
	"F3",		"kf3",
	"F4",		"kf4",
	"F5",		"kf5",
	"F6",		"kf6",
	"F7",		"kf7",
	"F8",		"kf8",
	"F9",		"kf9",
	"F10",		"kf10",
	"F11",		"kf11",
	"F12",		"kf12",
	"INSERT",	"kich1",
	"DELETE",	"kdch1",
	"HOME",		"khome",
	"END",		"kend",
	"PGUP",		"kpp",
	"PGDN",		"knp",
	"KEY_UP",	"kcuu1",
	"KEY_DOWN",	"kcud1",
	"KEY_LEFT",	"kcub1",
	"KEY_RIGHT",	"kcuf1"
]

funcs = [
	"T_ENTER_CA",		"smcup",
	"T_EXIT_CA",		"rmcup",
	"T_SHOW_CURSOR",	"cnorm",
	"T_HIDE_CURSOR",	"civis",
	"T_CLEAR_SCREEN",	"clear",
	"T_SGR",		"sgr",
	"T_SGR0",		"sgr0",
	"T_UNDERLINE",		"smul",
	"T_BOLD",		"bold",
	"T_BLINK",		"blink",
	"T_MOVE_CURSOR",	"cup",
	"T_ENTER_KEYPAD",	"smkx",
	"T_EXIT_KEYPAD",	"rmkx"
]

def iter_pairs(iterable):
	iterable = iter(iterable)
	while True:
		yield (next(iterable), next(iterable))

def do_term(term, nick):
	curses.setupterm(term)
	sys.stdout.write("/* %s */\n" % term)
	sys.stdout.write("static const char *%s_keys[] = {\n\t" % nick)
	for k,v in iter_pairs(keys):
		sys.stdout.write("\"")
		puttc(curses.tigetstr(v))
		sys.stdout.write("\",")
	sys.stdout.write(" 0\n};\n")
	sys.stdout.write("static const char *%s_funcs[] = {\n\t" % nick)
	for k,v in iter_pairs(funcs):
		#sys.stdout.write("\t[%s] = \"" % k)
		sys.stdout.write("\"")
		if v == "sgr":
			sys.stdout.write("\\033[3%u;4%um")
		elif v == "cup":
			sys.stdout.write("\\033[%u;%uH")
		else:
			tc = curses.tigetstr(v)
			puttc(tc)
		if v == "rmkx":
			sys.stdout.write("\"\n")
		else:
			sys.stdout.write("\",")
	sys.stdout.write("};\n\n")

def do_terms(d):
	sys.stdout.write("static struct term {\n")
	sys.stdout.write("\tconst char *name;\n")
	sys.stdout.write("\tconst char **keys;\n")
	sys.stdout.write("\tconst char **funcs;\n")
	sys.stdout.write("} terms[] = {\n")

	for k,v in d.items():
		sys.stdout.write("\t{\"%s\", %s_keys, %s_funcs},\n" % (k, v, v))
	sys.stdout.write("\t{0,0,0}\n};\n")

for k,v in terminals.items():
	do_term(k, v)

do_terms(terminals)

