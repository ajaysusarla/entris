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

#include <stdio.h>
#include <unistd.h>
#include <term.h>
#include <stdlib.h>
#include <getopt.h>
#include <libgen.h>
#include <errno.h>

#include "interface.h"
#include "game.h"

/* Global game instance */
Game *game;

/* Long options */
static struct option long_options [] = {
	{ "help", no_argument, NULL, 'h' },
	{ "version", no_argument, NULL, 'V' },
	{ NULL, 0, NULL, 0 }
};

static void usage(char *program_name)
{
	printf("%s - %s\n\n", PACKAGE, "A curses based tetris game.");
	printf("Usage: %s [OPTIONS...] \n", program_name);
	printf("   -h, --help,        display this help and exit\n");
	printf("   -V, --version,     output version information and exit\n\n");

	exit(EXIT_FAILURE);
}

static void help(char *program_name)
{
	printf("%s - %s\n\n", PACKAGE, "A curses based tetris game.");
	printf("**  Instructions  **\n\n");
	printf("    KEY         -       ACTION\n");
	printf("----------------------------------\n");
	printf("Left Arrow  :  Moves the block left.\n");
	printf("Right Arrow :  Moves the block right.\n");
	printf("Down Arrow  :  Moves the block down, faster.\n");
	printf("Up Arrow    :  Rotate block counter-clockwise.\n");
	printf("Space Bar   :  Rotate block clockwise.\n");
	printf("p/P         :  Pause the game.\n");
	printf("q/Q/C-c/Esc :  Quit the game.\n");

	printf("\n");

	printf("**     Options    **\n\n");
	printf("Usage: %s [OPTIONS...] \n", program_name);
	printf("   -h, --help,        display this help and exit\n");
	printf("   -V, --version,     output version information and exit\n\n");

	exit(EXIT_SUCCESS);
}

static void version(char *program_name)
{
	printf("%s %s\n\n", program_name, VERSION);

	printf("Written by %s.\n","Parthasarathi Susarla");

	exit(EXIT_SUCCESS);
}

static int parse_args(char *program_name, int argc, char **argv)
{
	int opt;
	//int optind = 0;
	int do_help;
	int do_version;

	/* Default values */
	do_help = FALSE;
	do_version = FALSE;

	while (1) {
		opt = getopt_long(argc, argv, "hV", long_options, NULL);
		if (opt == -1)
			break;

		switch (opt) {
		case '\0':
			break;

		case 'h':
			do_help = TRUE;
			break;

		case 'V':
			do_version = TRUE;
			break;

		default:
			usage(program_name);
			break;
		}
	}

	/* Version information is requested.  */
	if (do_version)
		version(program_name);

	/* Help is requested.  */
	if (do_help)
		help(program_name);

	return 0;
}

int main(int argc, char **argv)
{
	char *program_name;

	program_name = basename(argv[0]);

	parse_args(program_name, argc, argv);

	/* ncurses init */
	if (interface_init() == NULL) {
		perror("Error initialising ncurses.");
		exit(EXIT_FAILURE);
	}

	interface_init_input_opts();

	interface_hide_cursor(TRUE);

	game = game_new();

	if (game_create_board(game)) {
		fprintf (stderr, "entris: main: Could not create game board\n");
		game_free(game);
		exit(EXIT_FAILURE);
	}

	game_loop();

	game_free(game);

	interface_hide_cursor(FALSE);
	interface_fin();

	exit(EXIT_SUCCESS);
}
