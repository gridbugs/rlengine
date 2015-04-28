#include "drawing/curses_drawer.hpp"
#include <ncurses.h>

typedef enum {
    COL_WHITE = 16,
    COL_BLACK
} col_t;

typedef enum {
    PAIR_WALL = 16,
    PAIR_FLOOR
} pair_t;

curses_drawer::curses_drawer() {
    init_color(COL_WHITE, 1000, 1000, 1000);
    init_color(COL_BLACK, 0, 0, 0);

    init_pair(PAIR_WALL, COL_BLACK, COL_WHITE);
    init_pair(PAIR_FLOOR, COL_WHITE, COL_BLACK);
}

void curses_drawer::draw_cell(game_cell &c) {
    wmove(stdscr, c.y_coord, c.x_coord);
    if (c.is_opaque()) {
        wattron(stdscr, COLOR_PAIR(PAIR_WALL));
        waddch(stdscr, '#');
        wattroff(stdscr, COLOR_PAIR(PAIR_WALL));
    } else {
        wattron(stdscr, COLOR_PAIR(PAIR_FLOOR));
        waddch(stdscr, '.');
        wattroff(stdscr, COLOR_PAIR(PAIR_FLOOR));
    }
}
