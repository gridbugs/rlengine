#ifndef _CURSES_DRAWER_HPP_
#define _CURSES_DRAWER_HPP_
#include "drawing/drawer.hpp"
#include "samples/game_cell.hpp"
#include "samples/knowledge.hpp"
#include "io/curses.hpp"

typedef enum {
    COL_WHITE = 16,
    COL_GREY,
    COL_BLACK
} col_t;

typedef enum {
    PAIR_VISIBLE = 16,
    PAIR_REMEMBERED,
    PAIR_UNKNOWN,
    PAIR_BLUE,
    PAIR_RED,
    PAIR_GREEN,
    PAIR_CYAN,
    PAIR_WHITE,
    PAIR_MAGENTA
} pair_t;

template <typename C, typename W, typename K>
class curses_drawer : public drawer<C, W, K> {
    protected:
    virtual void draw_cell_from_actor(const W &cell, const K &kcell, const C &ctr) {
        wmove(curses::game_window, cell.y_coord, cell.x_coord);

        char ch;
        pair_t pair = PAIR_UNKNOWN;

        if (cell.coord == ctr.coord) {
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

    public:
    curses_drawer() {
        #define VISIBLE_VALUE 900
        #define REMEMBERED_VALUE 300
        init_color(COL_WHITE, VISIBLE_VALUE, VISIBLE_VALUE, VISIBLE_VALUE);
        init_color(COL_GREY, REMEMBERED_VALUE, REMEMBERED_VALUE, REMEMBERED_VALUE);
        init_color(COL_BLACK, 0, 0, 0);

        init_pair(PAIR_VISIBLE, COLOR_WHITE, COLOR_BLACK);
        init_pair(PAIR_REMEMBERED, COLOR_BLUE, COLOR_BLACK);
        init_pair(PAIR_UNKNOWN, COLOR_BLACK, COLOR_BLACK);
        init_pair(PAIR_BLUE, COLOR_BLUE, COLOR_BLACK);
        init_pair(PAIR_RED, COLOR_RED, COLOR_BLACK);
        init_pair(PAIR_GREEN, COLOR_GREEN, COLOR_BLACK);
        init_pair(PAIR_CYAN, COLOR_CYAN, COLOR_BLACK);
        init_pair(PAIR_WHITE, COLOR_WHITE, COLOR_BLACK);
        init_pair(PAIR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    }
    void draw_world_from_actor(world<C, W> &w, const actor_drawing_interface<C, K> &a) {
        const C &ch = a.get_character();
        const grid<K> &knowledge_grid = a.get_knowledge_grid();
        const grid<W> &game_grid = w.maps[ch.level_index];

        game_grid.for_each([&](const W &c) {
            draw_cell_from_actor(c, knowledge_grid.get_cell(c.coord), ch);
        });
    }
};

#endif
