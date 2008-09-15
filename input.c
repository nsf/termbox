#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "term.h"

#define BUFFER_SIZE_MAX 16
#define SUCCESS 0
#define FAILURE -1

static const unsigned char utf8_length[256] = {
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,6,6,1,1
};

static const unsigned char utf8_mask[6] = {
	0x7F,
	0x1F,
	0x0F,
	0x07,
	0x03,
	0x01
};

static unsigned char utf8_char_length(char c)
{
	return utf8_length[(unsigned char)c];
}

static uint32_t utf8_char_to_unicode(const char *c)
{
	int i;
	unsigned char len = utf8_char_length(*c);
	unsigned char mask = utf8_mask[len-1];
	uint32_t result = c[0] & mask;
	for (i = 1; i < len; ++i) {
		result <<= 6;
		result |= c[i] & 0x3f;
	}

	return result;
}

/* if s1 starts with s2 returns 1, else 0 */
static int starts_with(const char *s1, const char *s2)
{
	/* nice huh? */
	while (*s2) if (*s1++ != *s2++) return 0; return 1;
}

/* convert escape sequence to event, and return consumed bytes on success (failure == 0) */
static int parse_escape_seq(struct tb_key_event *event, const char *buf)
{
	/* it's pretty simple here, find 'starts_with' match and return success, else return failure */
	int i;
	for (i = 0; keys[i]; i++) {
		if (starts_with(buf, keys[i])) {
			event->ch = 0;
			event->key = 0xFFFF-i;
			return strlen(keys[i]);
		}
	}
	return 0;
}

int extract_event(struct tb_key_event *event, struct ringbuffer *inbuf, int inputmode)
{
	char buf[BUFFER_SIZE_MAX+1];
	int nbytes = ringbuffer_data_size(inbuf);

	if (nbytes > BUFFER_SIZE_MAX) 
		nbytes = BUFFER_SIZE_MAX;

	if (nbytes == 0)
		return FAILURE;

	ringbuffer_read(inbuf, buf, nbytes);
	buf[nbytes] = '\0';

	if (buf[0] == '\033') {
		int n = parse_escape_seq(event, buf);
		if (n) {
			ringbuffer_pop(inbuf, 0, n);
			return SUCCESS;
		} else {
			/* it's not escape sequence, then it's ALT or ESC, check inputmode */
			switch (inputmode) {
			case TB_INPUT_ESC:
				/* if we're in escape mode, fill ESC event, pop buffer, return success */
				event->ch = 0;
				event->key = TB_KEY_ESC;
				event->mod = 0;
				ringbuffer_pop(inbuf, 0, 1);
				return SUCCESS;
				break;
			case TB_INPUT_ALT:
				/* if we're in alt mode, set ALT modifier to event and redo parsing */
				event->mod = TB_MOD_ALT;
				ringbuffer_pop(inbuf, 0, 1);
				return extract_event(event, inbuf, inputmode);
				break;
			default:
				assert(!"never got here");
				break;
			}
		}
	}

	/* if we're here, this is not an escape sequence and not an alt sequence 
	 * so, it's a FUNCTIONAL KEY or a UNICODE character
	 */

	/* first of all check if it's a functional key */
	if ((unsigned char)buf[0] <= TB_KEY_SPACE || 
	    (unsigned char)buf[0] == TB_KEY_BACKSPACE2) 
	{
		/* fill event, pop buffer, return success */
		event->ch = 0;
		event->key = (uint16_t)buf[0];
		ringbuffer_pop(inbuf, 0, 1);
		return SUCCESS;
	}

	/* feh... we got utf8 here */
	
	/* check if there is all bytes */
	if (nbytes >= utf8_char_length(buf[0])) {
		/* everything ok, fill event, pop buffer, return success */
		event->ch = utf8_char_to_unicode(buf);
		event->key = 0;
		ringbuffer_pop(inbuf, 0, utf8_char_length(buf[0]));
		return SUCCESS;
	}

	/* fuck!!!!1111odin1odinodin */
	return FAILURE;
}
