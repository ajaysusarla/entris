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

#ifndef GAME_H
#define GAME_H

#include <ncurses.h>

#define GAME_BOARD_HEIGHT 20
#define GAME_BOARD_WIDTH  10
#define GAME_BOARD_X_POS  5
#define GAME_BOARD_Y_POS  5

#define GAME_DLG_COLOR(dialog, f, b, h)		\
	do {					\
		game->dlg.dialog.fg = (f);	\
		game->dlg.dialog.bg = (b);	\
		game->dlg.dialog.hl = (h);	\
	} while (0)

#define MIN(x,y) (x < y ? x : y)

typedef enum _GameBlockType GameBlockType;
typedef struct _GameWidgetColour GameWidgetColour;
typedef struct _GameDialogInfo GameDialogInfo;
typedef struct _Game Game;

enum _GameBlockType {
	GAME_BLOCK_I,
	GAME_BLOCK_J,
	GAME_BLOCK_L,
	GAME_BLOCK_T,
	GAME_BLOCK_Z,
	GAME_NUM_BLOCKS
};

struct _GameWidgetColour {
	int fg;  /* foreground colour */
	int bg;  /* background colour */
	int hl;  /* highlight this widget */
	chtype attr; /* colour attribute */
};

struct _GameDialogInfo {
	const char *maintitle;
	GameWidgetColour screen;
	GameWidgetColour shadow;
	GameWidgetColour dialog;
	GameWidgetColour title;
	GameWidgetColour border;
	GameWidgetColour button_active;
	GameWidgetColour button_inactive;
	GameWidgetColour button_label_active;
	GameWidgetColour button_label_inactive;
	GameWidgetColour menubox;
	GameWidgetColour menubox_border;
	GameWidgetColour uarrow;
	GameWidgetColour darrow;
};

struct _Game {
	WINDOW *main_win;
	WINDOW *score;
	WINDOW *level;
	GameDialogInfo dlg;
};

Game *game_new(void);
void game_free(Game *game);
void game_loop(void);
int game_create_board(Game *game);

#endif /* GAME_H */
