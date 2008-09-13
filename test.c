#include <stdio.h>
#include "termbox.h"

int main(int argc, char **argv)
{
	uint16_t color = TB_YELLOW | TB_BOLD;
	printf("%d %d\n", color & 0x0F, color & 0xF0);
	return 0;
}
