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

#include <string.h>
#include <stdlib.h>

static void game_init_single_colour(GameWidgetColour *colour)
{
	static int pair = 0;

	pair++;

	init_pair(pair, colour->fg, colour->bg);
	if (colour->hl)
		colour->attr = A_BOLD | COLOR_PAIR(pair);
	else
		colour->attr = COLOR_PAIR(pair);
}

static void game_window_init_colours(Game *game)
{
	GameDialogInfo dlg;

	dlg = game->dlg;

	game_init_single_colour(&dlg.screen);
	game_init_single_colour(&dlg.shadow);
	game_init_single_colour(&dlg.dialog);
	game_init_single_colour(&dlg.title);
	game_init_single_colour(&dlg.border);
	game_init_single_colour(&dlg.button_active);
	game_init_single_colour(&dlg.button_inactive);
	game_init_single_colour(&dlg.button_label_active);
	game_init_single_colour(&dlg.button_label_inactive);
	game_init_single_colour(&dlg.menubox);
	game_init_single_colour(&dlg.menubox_border);
	game_init_single_colour(&dlg.uarrow);
	game_init_single_colour(&dlg.darrow);
}

static void game_setup_mono_theme(Game *game)
{
	game->dlg.screen.attr = A_NORMAL;
	game->dlg.shadow.attr = A_NORMAL;
	game->dlg.dialog.attr = A_NORMAL;
	game->dlg.title.attr = A_BOLD;
	game->dlg.border.attr = A_NORMAL;
	game->dlg.button_active.attr = A_REVERSE;
	game->dlg.button_inactive.attr = A_DIM;
	game->dlg.button_label_active.attr = A_REVERSE;
	game->dlg.button_label_inactive.attr = A_NORMAL;
	game->dlg.menubox.attr = A_NORMAL;
	game->dlg.menubox_border.attr = A_NORMAL;
	game->dlg.uarrow.attr = A_BOLD;
	game->dlg.darrow.attr = A_BOLD;
}

static void game_setup_blue_theme(Game *game)
{
	GAME_DLG_COLOR(screen, COLOR_CYAN, COLOR_BLUE, true);
	GAME_DLG_COLOR(shadow, COLOR_BLACK, COLOR_BLACK, true);
	GAME_DLG_COLOR(dialog, COLOR_BLACK, COLOR_WHITE, false);
	GAME_DLG_COLOR(title, COLOR_BLUE, COLOR_WHITE, true);
	GAME_DLG_COLOR(border, COLOR_WHITE, COLOR_WHITE, true);
	GAME_DLG_COLOR(button_active, COLOR_WHITE, COLOR_BLUE, true);
	GAME_DLG_COLOR(button_inactive, COLOR_BLACK, COLOR_WHITE, false);
	GAME_DLG_COLOR(button_label_active, COLOR_WHITE, COLOR_BLUE, true);
	GAME_DLG_COLOR(button_label_inactive, COLOR_BLACK, COLOR_WHITE, true);
	GAME_DLG_COLOR(menubox, COLOR_BLACK, COLOR_WHITE, false);
	GAME_DLG_COLOR(menubox_border, COLOR_WHITE, COLOR_WHITE, true);
	GAME_DLG_COLOR(uarrow, COLOR_GREEN, COLOR_WHITE, true);
	GAME_DLG_COLOR(darrow, COLOR_GREEN, COLOR_WHITE, true);
}

static void game_setup_colour_theme(Game *game)
{
#ifdef USE_COLOURS
	if (has_colors()) {
		game_setup_blue_theme(game);
		start_color();
		game_window_init_colours(game);
	}
#else
	game_setup_mono_theme(game);
#endif
}

/*
 * Set window to attribute 'attr'
 */
static void attr_clear(WINDOW * win, int height, int width, chtype attr)
{
	int i, j;

	wattrset(win, attr);

	for (i = 0; i < height; i++) {
		wmove(win, i, 0);
		for (j = 0; j < width; j++)
			waddch(win, ' ');
	}

	touchwin(win);
}


static void game_dialog_clear(Game *game)
{
	attr_clear(stdscr, LINES, COLS, game->dlg.screen.attr);

	if (game->dlg.maintitle != NULL) {
		int i;

		wattrset(stdscr, game->dlg.screen.attr);
		mvwaddstr(stdscr, 0, 1, (char *)game->dlg.maintitle);
		wmove(stdscr, 1, 1);
		for (i = 1; i < COLS - 1; i++)
			waddch(stdscr, ACS_HLINE);
	}

	wnoutrefresh(stdscr);
}

static void game_window_draw_shadow(WINDOW *win,
				    chtype attr,
				    int y, int x,
				    int height, int width)
{
#ifdef USE_COLOURS
	int i;
	if (has_colors()) {
		wattrset(win, attr);
		wmove(win, y + height, x + 2);
		for (i = 0; i < width; i++)
			waddch(win, winch(win) & A_CHARTEXT);
		for (i = y + 1; i < y + height + 1; i++) {
			wmove(win, i, x + width);
			waddch(win, winch(win) & A_CHARTEXT);
			waddch(win, winch(win) & A_CHARTEXT);
		}
		wnoutrefresh(win);
	}
#endif
}

static void game_window_draw_box(WINDOW *win,
				 int y, int x,
				 int height, int width,
				 chtype box, chtype border)
{
	int i, j;

	wattrset(win, 0);
	for (i = 0; i < height; i++) {
		wmove(win, y + i, x);
		for (j = 0; j < width -2; j++)
			if (!i && !j)
				waddch(win, border | ACS_ULCORNER);
			else if (i == height - 1 && !j)
				waddch(win, border | ACS_LLCORNER);
			else if (!i && j == width - 3)
				waddch(win, box | ACS_URCORNER);
			else if (i == height - 1 && j == width - 1)
				waddch(win, box | ACS_LRCORNER);
			else if (!i)
				waddch(win, border | ACS_HLINE);
			else if (i == height - 1)
				waddch(win, box | ACS_HLINE);
			else if (!j)
				waddch(win, border | ACS_VLINE);
			else if (j == width - 3)
				waddch(win, box | ACS_VLINE);
			else
				waddch(win, box | ' ');
	}
}

static void game_window_set_title(Game *game, const char *title, int width)
{
	if (title) {
		int len = MIN(width - 2, strlen(title));
		wattrset(game->main_win, game->dlg.title.attr);
		mvwaddch(game->main_win, 0 , (width - len)/2 - 1, ' ');
		mvwaddnstr(game->main_win, 0, (width - len)/2, title, len);
		waddch(game->main_win, ' ');
	}
}

Game *game_new(void)
{
	Game *game;

	game = (Game *)calloc(1, sizeof(Game));
	if (game == NULL) {
		fprintf(stderr, "entris:entris_init: calloc() failed.\n");
	}

	return game;
}

void game_free(Game *game)
{
	if (game != NULL) {
		if (game->main_win)
			delwin(game->main_win);

		free(game);
		game = NULL;
	}

	return;
}

int game_create_board(Game *game)
{
	int height, width;
	int menu_height, menu_width;
	int xpos, ypos;
	int boxx, boxy;

	int i;

	game_setup_colour_theme(game);

	/* terminal resolution */
	getmaxyx(stdscr, height, width);

	if (height <15 || width < 60) {
		fprintf(stderr, "entris: display too small.\n");
		return -1;
	}

	game->dlg.maintitle = "entrix - A curses based Tetris game";

	game_dialog_clear(game);

	height -=4;
	width -=5;

	menu_height = height - 20;
	menu_width = width - 16;

	/* We need the actual tetris board at the center of the screen */
	xpos = (COLS - width) / 2;
	ypos = (LINES - height) / 2;

	game_window_draw_shadow(stdscr, game->dlg.shadow.attr,
				ypos, xpos, height, width);
	game->main_win = newwin(height, width, ypos, xpos);
	keypad(game->main_win, TRUE);

	/* Draw a box */
	game_window_draw_box(game->main_win,
			     ypos, xpos,
			     height,width,
			     game->dlg.dialog.attr,
			     game->dlg.border.attr);

	wattrset(game->main_win, game->dlg.border.attr);
 	mvwaddch(game->main_win, height - 1, 2, ACS_LLCORNER);

	for (i = 0; i < width - 2 ; i++)
		waddch(game->main_win, ACS_HLINE);

	wattrset(game->main_win, game->dlg.dialog.attr);
	wbkgdset(game->main_win, game->dlg.dialog.attr & A_COLOR);
	waddch(game->main_win, ACS_LRCORNER);

	/* Title for the window */
	game_window_set_title(game, "Entris", width);

	wattrset(game->main_win, game->dlg.dialog.attr);

	boxy = height - menu_height - 5;
	boxx = (width - menu_width) / 2 - 1;


	/* Create a new window for the stats/menu */
	game->score = subwin(game->main_win, menu_height, menu_width,
			     ypos + boxy + 1, xpos + boxx + 1);
	keypad(game->score, TRUE);

	game_window_draw_box(game->main_win, boxy, boxx,
				  menu_height + 2, menu_width + 2,
				  game->dlg.menubox_border.attr,
				  game->dlg.menubox.attr);

 	mvwaddch(game->main_win,
		 boxy + menu_height + 1, menu_width + 6,
		 ACS_LRCORNER);

	/* */
	wnoutrefresh(game->main_win);
	wrefresh(game->score);
	refresh();

	return 0;
}
