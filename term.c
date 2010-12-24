#include <stdlib.h>
#include <string.h>

#include "term.h"

/* screen */
static const char *screen_keys[] = {
	"\033OP","\033OQ","\033OR","\033OS","\033[15~","\033[17~","\033[18~","\033[19~","\033[20~","\033[21~","\033[23~","\033[24~","\033[2~","\033[3~","\033[1~","\033[4~","\033[5~","\033[6~","\033OA","\033OB","\033OD","\033OC", 0
};
static const char *screen_funcs[] = {
	"\033[?1049h","\033[?1049l","\033[34h\033[?25h","\033[?25l","\033[H\033[J","\033[3%u;4%um","\033[m","\033[4m","\033[1m","\033[5m","\033[%u;%uH","\033[?1h\033=","\033[?1l\033>"
};

/* rxvt-unicode */
static const char *rxvt_unicode_keys[] = {
	"\033OP","\033OQ","\033OR","\033OS","\033[15~","\033[17~","\033[18~","\033[19~","\033[20~","\033[21~","\033[23~","\033[24~","\033[2~","\033[3~","\033[1~","\033[4~","\033[5~","\033[6~","\033OA","\033OB","\033OD","\033OC", 0
};
static const char *rxvt_unicode_funcs[] = {
	"\033[?1049h","\033[?1049l","\033[34h\033[?25h","\033[?25l","\033[H\033[J","\033[3%u;4%um","\033[m","\033[4m","\033[1m","\033[5m","\033[%u;%uH","\033[?1h\033=","\033[?1l\033>"
};

/* xterm */
static const char *xterm_keys[] = {
	"\033OP","\033OQ","\033OR","\033OS","\033[15~","\033[17~","\033[18~","\033[19~","\033[20~","\033[21~","\033[23~","\033[24~","\033[2~","\033[3~","\033[1~","\033[4~","\033[5~","\033[6~","\033OA","\033OB","\033OD","\033OC", 0
};
static const char *xterm_funcs[] = {
	"\033[?1049h","\033[?1049l","\033[34h\033[?25h","\033[?25l","\033[H\033[J","\033[3%u;4%um","\033[m","\033[4m","\033[1m","\033[5m","\033[%u;%uH","\033[?1h\033=","\033[?1l\033>"
};

/* Eterm */
static const char *Eterm_keys[] = {
	"\033OP","\033OQ","\033OR","\033OS","\033[15~","\033[17~","\033[18~","\033[19~","\033[20~","\033[21~","\033[23~","\033[24~","\033[2~","\033[3~","\033[1~","\033[4~","\033[5~","\033[6~","\033OA","\033OB","\033OD","\033OC", 0
};
static const char *Eterm_funcs[] = {
	"\033[?1049h","\033[?1049l","\033[34h\033[?25h","\033[?25l","\033[H\033[J","\033[3%u;4%um","\033[m","\033[4m","\033[1m","\033[5m","\033[%u;%uH","\033[?1h\033=","\033[?1l\033>"
};

/* linux */
static const char *linux_keys[] = {
	"\033OP","\033OQ","\033OR","\033OS","\033[15~","\033[17~","\033[18~","\033[19~","\033[20~","\033[21~","\033[23~","\033[24~","\033[2~","\033[3~","\033[1~","\033[4~","\033[5~","\033[6~","\033OA","\033OB","\033OD","\033OC", 0
};
static const char *linux_funcs[] = {
	"\033[?1049h","\033[?1049l","\033[34h\033[?25h","\033[?25l","\033[H\033[J","\033[3%u;4%um","\033[m","\033[4m","\033[1m","\033[5m","\033[%u;%uH","\033[?1h\033=","\033[?1l\033>"
};

static struct term {
	const char *name;
	const char **keys;
	const char **funcs;
} terms[] = {
	{"screen", screen_keys, screen_funcs},
	{"rxvt-unicode", rxvt_unicode_keys, rxvt_unicode_funcs},
	{"xterm", xterm_keys, xterm_funcs},
	{"Eterm", Eterm_keys, Eterm_funcs},
	{"linux", linux_keys, linux_funcs},
	{0,0,0}
};

const char **keys;
const char **funcs;

static int try_compatible(const char *term, const char *name,
			  const char **tkeys, const char **tfuncs)
{
	if (strstr(term, name)) {
		keys = tkeys;
		funcs = tfuncs;
		return 0;
	}

	return EUNSUPPORTED_TERM;
}

int init_term()
{
	int i;
	const char *term = getenv("TERM");
	for (i = 0; terms[i].name; i++) {
		if (!strcmp(terms[i].name, term)) {
			keys = terms[i].keys;
			funcs = terms[i].funcs;
			return 0;
		}
	}

	/* let's do some heuristic, maybe it's a compatible terminal */
	if (try_compatible(term, "xterm", xterm_keys, xterm_funcs) == 0)
		return 0;
	if (try_compatible(term, "rxvt", rxvt_unicode_keys, rxvt_unicode_funcs) == 0)
		return 0;
	if (try_compatible(term, "linux", linux_keys, linux_funcs) == 0)
		return 0;
	if (try_compatible(term, "Eterm", Eterm_keys, Eterm_funcs) == 0)
		return 0;
	if (try_compatible(term, "screen", screen_keys, screen_funcs) == 0)
		return 0;

	return EUNSUPPORTED_TERM;
}
