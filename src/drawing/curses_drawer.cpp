#include "drawing/curses_drawer.hpp"
#include <ncurses.h>
#include "io/curses.hpp"
#include <vector>

typedef enum {
    COL_WHITE = 16,
    COL_BLACK,
    COL_DARK_BLUE,
    COL_DARK_GREY,
    COL_LIGHT_GREY
} col_t;

typedef enum {
    PAIR_WALL = 16,
    PAIR_FLOOR,
    PAIR_UNKNOWN,
    PAIR_WALL_REMEMBERED,
    PAIR_FLOOR_REMEMBERED
} pair_t;

static void add_char(char ch, int pair) {
    wattron(curses::game_window, COLOR_PAIR(pair));
    waddch(curses::game_window, ch);
    wattroff(curses::game_window, COLOR_PAIR(pair));
}

curses_drawer::curses_drawer() :
    floor_range(64, curses_col(0, 0, 800), curses_col(0, 0, 200)),
    wall_range(64, curses_col(900, 900, 900), curses_col(200, 200, 200))
{
    init_color(COL_WHITE, 1000, 1000, 1000);
    init_color(COL_BLACK, 0, 0, 0);
    init_color(COL_DARK_BLUE, 0, 0, 200);
    init_color(COL_DARK_GREY, 250, 250, 250);
    init_color(COL_LIGHT_GREY, 750, 750, 750);

    init_pair(PAIR_WALL, COL_BLACK, COL_WHITE);
    init_pair(PAIR_FLOOR, COL_WHITE, COL_DARK_GREY);
    init_pair(PAIR_UNKNOWN, COL_BLACK, COL_BLACK);
    init_pair(PAIR_WALL_REMEMBERED, COL_DARK_GREY, COL_LIGHT_GREY);
    init_pair(PAIR_FLOOR_REMEMBERED, COL_LIGHT_GREY, COL_DARK_BLUE);

    col_table.map_range(64, floor_range);
    col_table.map_range(128, wall_range);
}

void curses_drawer::draw_cell(game_cell &c) {
    wmove(curses::game_window, c.y_coord, c.x_coord);
    if (c.is_opaque()) {
        wattron(curses::game_window, COLOR_PAIR(PAIR_WALL));
        waddch(curses::game_window, '#');
        wattroff(curses::game_window, COLOR_PAIR(PAIR_WALL));
    } else {
        wattron(curses::game_window, COLOR_PAIR(PAIR_FLOOR));
        waddch(curses::game_window, '.');
        wattroff(curses::game_window, COLOR_PAIR(PAIR_FLOOR));
    }
}

void curses_drawer::draw_cell(game_cell &c, knowledge_cell &k, character &ch) {
    
}

void curses_drawer::draw_cell(game_cell &c, knowledge_cell &k) {

    wmove(curses::game_window, c.y_coord, c.x_coord);

    if (k.is_unknown()) {
        add_char(' ', PAIR_UNKNOWN);
    } else if (k.is_visible()) {
        if (c.is_opaque()) {
            add_char('#', PAIR_WALL);
        } else {
            add_char('.', PAIR_FLOOR);
        }
    } else {
        if (c.is_opaque()) {
            add_char('#', PAIR_WALL_REMEMBERED);
        } else {
            add_char('.', PAIR_FLOOR_REMEMBERED);
        }
    }


}

void curses_drawer::draw_world(world &w, behaviour &b) {
    drawer::draw_world(w, b);
    wrefresh(curses::game_window);
}

void curses_drawer::draw_world(world &w) {
    drawer::draw_world(w);
    wrefresh(curses::game_window);
}

void curses_drawer::draw_world(world &w, behaviour &b, character &ch) {
    grid<game_cell> &g = w.map;
    const grid<knowledge_cell> &k = b.get_knowledge_grid();
    for (int i = 0; i < g.height; ++i) {
        for (int j = 0; j < g.width; ++j) {
            draw_cell(g[i][j], k[i][j], ch);
        }
    }
}
