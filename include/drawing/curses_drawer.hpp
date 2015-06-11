#ifndef _CURSES_DRAWER_HPP_
#define _CURSES_DRAWER_HPP_

#include "drawing/drawer.hpp"
#include "world/world_cell.hpp"
#include "knowledge/knowledge.hpp"
#include "io/curses.hpp"
#include "drawing/curses_col.hpp"

class curses_drawer : public drawer {
    protected:
    virtual void draw_cell_from_actor(const world_cell &cell, const knowledge_cell &kcell, const character &ctr) {
        wmove(curses::game_window, cell.y_coord, cell.x_coord);

        char ch;
        pair_t pair = PAIR_UNKNOWN;
        bool bold = false;

        if (kcell.contains_character()) {
            ch = kcell.characters.front().get_char();
            bold = true;
        } else if (cell.is_opaque()) {
            ch = '#';
        } else {
            ch = '.';
        }

        if (kcell.is_visible()) {
            if (kcell.contains_character()) {
                pair = kcell.characters.front().get_col_pair_visible(); 
            } else {
                pair = PAIR_VISIBLE;
            }
        } else if (kcell.is_remembered()) {
            if (kcell.contains_character()) {
                pair = kcell.characters.front().get_col_pair_remembered();
            } else {
                pair = PAIR_REMEMBERED;
            }
        } else {
            pair = PAIR_UNKNOWN;
        }

        if (bold) {
            wattron(curses::game_window, A_BOLD);
        }
        wattron(curses::game_window, COLOR_PAIR(pair));
        waddch(curses::game_window, ch);
        wattroff(curses::game_window, COLOR_PAIR(pair));
        if (bold) {
            wattroff(curses::game_window, A_BOLD);
        }
    }

    void draw_hud_from_actor(world &w, const actor_drawing_interface &a) {
        wclear(curses::hud_window);

        pair_t pair = a.get_character().get_col_pair_visible(); 
        wattron(curses::hud_window, A_BOLD);
        wattron(curses::hud_window, COLOR_PAIR(pair));
        wprintw(curses::hud_window, "%c\n", a.get_character().get_char());
        wattroff(curses::hud_window, COLOR_PAIR(pair));
        wattroff(curses::hud_window, A_BOLD);
        
        wprintw(curses::hud_window, "%d/%d\n", 
            a.get_character().current_hit_points,
            a.get_character().max_hit_points
        );

        wprintw(curses::hud_window, "\n");

        a.get_character().get_current_knowledge_grid().
            for_each_visible_character([&](const character_image &ci) {

            if (ci.source != &(a.get_character()) && ci.current_hit_points > 0) {

                pair_t pair = ci.get_col_pair_visible(); 
                wattron(curses::hud_window, A_BOLD);
                wattron(curses::hud_window, COLOR_PAIR(pair));
                wprintw(curses::hud_window, "%c\n", ci.get_char());
                wattroff(curses::hud_window, COLOR_PAIR(pair));
                wattroff(curses::hud_window, A_BOLD);
                wprintw(curses::hud_window, "%d/%d\n", 
                    ci.current_hit_points,
                    ci.max_hit_points
                );
                
                wprintw(curses::hud_window, "\n");
            }
        });
        
        wrefresh(curses::hud_window);
    }

    public:
    curses_drawer() {
        #define VISIBLE_VALUE 800
        #define REMEMBERED_VALUE 200
        init_color(COL_LIGHT_GREY, VISIBLE_VALUE, VISIBLE_VALUE, VISIBLE_VALUE);
        init_color(COL_GREY, REMEMBERED_VALUE, REMEMBERED_VALUE, REMEMBERED_VALUE);
        init_color(COL_BLACK, 0, 0, 0);
        init_color(COL_RED, 800, 0, 0);
        init_color(COL_DARK_RED, 200, 0, 0);
        init_color(COL_WHITE, 1000, 1000, 1000);

        init_pair(PAIR_VISIBLE, COL_LIGHT_GREY, COL_BLACK);
        init_pair(PAIR_REMEMBERED, COL_GREY, COL_BLACK);
        init_pair(PAIR_UNKNOWN, COL_BLACK, COL_BLACK);
        init_pair(PAIR_BLUE, COLOR_BLUE, COL_BLACK);
        init_pair(PAIR_RED, COL_RED, COL_BLACK);
        init_pair(PAIR_DARK_RED, COL_DARK_RED, COL_BLACK);
        init_pair(PAIR_GREEN, COLOR_GREEN, COL_BLACK);
        init_pair(PAIR_CYAN, COLOR_CYAN, COL_BLACK);
        init_pair(PAIR_WHITE, COLOR_WHITE, COL_BLACK);
    }
    void draw_world_from_actor(world &w, const actor_drawing_interface &a) {
        const character &ch = a.get_character();
        const grid<knowledge_cell> &knowledge_grid = a.get_knowledge_grid();
        const grid<world_cell> &game_grid = w.maps[ch.level_index];

        game_grid.for_each([&](const world_cell &c) {
            draw_cell_from_actor(c, knowledge_grid.get_cell(c.coord), ch);
        });

        draw_hud_from_actor(w, a);
    }
};

#endif
