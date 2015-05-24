#include "drawing/curses_drawer.hpp"
#include "io/curses.hpp"

typedef enum {
    COL_WHITE = 16,
    COL_GREY,
    COL_BLACK
} col_t;

typedef enum {
    PAIR_VISIBLE = 16,
    PAIR_REMEMBERED,
    PAIR_UNKNOWN
} pair_t;

curses_drawer::curses_drawer() {
    init_color(COL_WHITE, 1000, 1000, 1000);
    init_color(COL_GREY, 250, 250, 250);
    init_color(COL_BLACK, 0, 0, 0);

    init_pair(PAIR_VISIBLE, COL_WHITE, COL_BLACK);
    init_pair(PAIR_REMEMBERED, COL_GREY, COL_BLACK);
    init_pair(PAIR_UNKNOWN, COL_BLACK, COL_BLACK);

}

static void draw_cell(game_cell &c) {
    wmove(curses::game_window, c.y_coord, c.x_coord);
    char ch;
    if (c.is_opaque()) {
        ch = '#';
    } else {
        ch = '.';
    }

    waddch(curses::game_window, ch);
}

static void draw_cell_from_actor(const game_cell &cell, const knowledge_cell &kcell, const character &ctr) {
    wmove(curses::game_window, cell.y_coord, cell.x_coord);

    char ch;
    pair_t pair = PAIR_UNKNOWN;

    if (cell.coord == ctr.position) {
        ch = '@';
        pair = PAIR_VISIBLE;
    } else {
        
        if (cell.is_opaque()) {
            ch = '#';
        } else {
            ch = '.';
        }
        
        if (kcell.is_visible()) {
            pair = PAIR_VISIBLE;
        } else if (kcell.is_remembered()) {
            pair = PAIR_REMEMBERED;
        } else {
            pair = PAIR_UNKNOWN;
        }
        
    }

    wattron(curses::game_window, COLOR_PAIR(pair));
    waddch(curses::game_window, ch);
    wattroff(curses::game_window, COLOR_PAIR(pair));

}

void curses_drawer::draw_world_from_actor(world &w, const actor_drawing_interface &a) {
    const character &ch = a.get_character();
    const grid<knowledge_cell> &knowledge_grid = a.get_knowledge_grid();
    const grid<game_cell> &game_grid = w.map;

    game_grid.for_each([knowledge_grid, ch](const game_cell &c) {
        draw_cell_from_actor(c, knowledge_grid.get_cell(c.coord), ch);
    });
}

void curses_drawer::draw_world(world &w) {
    w.map.for_each(draw_cell);
}

void curses_drawer::draw_hud(world &w, const hud &h) {
    werase(curses::hud_window);
    wprintw(curses::hud_window, "Time: %lld\n", h.get_time());
    wrefresh(curses::hud_window);
}
