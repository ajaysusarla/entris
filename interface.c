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


#include "interface.h"


/* Initialise interface - wrapper for interface */
WINDOW *interface_init(void)
{
	return initscr();
}

/* end the ncurses window */
void interface_fin(void)
{
	endwin();
}

/* Initialise input options */
void interface_init_input_opts(void)
{
	keypad (stdscr, TRUE);  /* Enable keypad for the user's terminal */
	nodelay (stdscr, TRUE); /* non-blocking mode*/
	cbreak ();              /* Read single input chars, without waiting for \n*/
	noecho ();              /* Don't echo input */
}

/* toggle cursor display */
void interface_hide_cursor(int state)
{
	if (state == TRUE)
		curs_set(0);
	else if (state == FALSE)
		curs_set(1);
	else
		printf("interface_hide_cursor: Invalid state\n");
}
