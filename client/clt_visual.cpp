// clt_visual.cpp
//
// Author: Noah BEAUFILS
// Date: 10-oct-2023

#include "clt.hpp"

/* \0337 -> save current state */
/* \0338 -> restore the most recent state */
/* \033[J -> erase from cursor to end */
/* \r -> return to the beginning of the line */

char	clt_extract_char(void) {

	char	c;
	struct termios	oldt;
	struct termios	newt;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;

	newt.c_lflag &= ~(ICANON | ECHO);

	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	c = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return c;
}