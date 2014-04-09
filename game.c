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

#include "game.h"

/* The game loop - where all the action takes place */
void game_loop(void)
{
	int ch = 0;

	nodelay(stdscr, FALSE);

	while (1) {
		ch = getch();

		if (ch == 'q' || ch == 'Q' || ch == 27)
			break;

		refresh();
	}

	nodelay(stdscr, FALSE);
}
