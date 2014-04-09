/*
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "util.h"

#include <stdio.h>
#include <term.h>
#include <ncurses.h>
#include <sys/ioctl.h>


void entris_get_screensize_using_termcap(short *widthp, short *heightp)
{
	*widthp = 0;
	*heightp = 0;

	if ((*heightp = (short)tgetnum("li")) == -1) {
		fprintf(stderr, "Failed to get termcap entry for lines\n");
		return;
	}

	if ((*widthp = (short)tgetnum("co")) == -1) {
		fprintf(stderr, "Failed to get termcap entry for columns\n");
		return;
	}
}

void entris_get_screensize_using_ioctl(int *widthp, int *heightp)
{
#ifdef TIOCGWINSZ
	struct winsize size;

	*widthp = 0;
	*heightp = 0;

	if (ioctl(0, TIOCGWINSZ, &size) < 0)
		return;

	*widthp = size.ws_col;
	*heightp = size.ws_row;
#else
	*widthp = 0;
	*heightp = 0;
#endif
}