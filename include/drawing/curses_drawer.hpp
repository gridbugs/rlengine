#ifndef _CURSES_DRAWER_HPP_
#define _CURSES_DRAWER_HPP_
#include <ncurses.h>
#include "drawing/drawer.hpp"
#include "actor/character.hpp"

#define N_COLS 256

class curses_col {
    public:
    const unsigned int r, g, b;
    curses_col(const unsigned int r, const unsigned int g, const unsigned int b) :
        r(r), g(g), b(b)
    {}
};

class curses_col_range {
    public:
    std::vector<curses_col> cols;
    curses_col_range(unsigned int n_cols, 
                     const curses_col &start, 
                     const curses_col &end)
    {
        const int dr = end.r - start.r;
        const int dg = end.g - start.g;
        const int db = end.b - start.b;

        const int steps = n_cols - 1;
        
        for (unsigned int i = 0; i < n_cols; ++i) {
            const int r = start.r + (dr * i) / steps;
            const int g = start.g + (dg * i) / steps;
            const int b = start.b + (db * i) / steps;
            
            cols.push_back(curses_col(r, g, b));
        }
    }
};

class curses_col_table {
    protected:

    std::array<const curses_col*, N_COLS> table;
    const int n_reserved = 16;

    public:
    curses_col_table() {
        for (unsigned int i = 0; i < table.size(); ++i) {
            table[i] = nullptr;
        }
    }

    void map_col(unsigned int idx, const curses_col &c) {
        table[idx] = &c;
        init_color(idx, c.r, c.g, c.b);
    }

    void map_range(unsigned int idx, const curses_col_range &range) {
        for (unsigned int i = 0; i < range.cols.size(); ++i) {
            map_col(idx + i, range.cols[i]);
        }
    }
};




class curses_drawer : public drawer {
    private:
    curses_col_table col_table;
    curses_col_range floor_range;
    curses_col_range wall_range;
    public:
    curses_drawer();
    void draw_cell(game_cell &c);
    void draw_cell(game_cell &c, knowledge_cell &k);
    void draw_cell(game_cell &c, knowledge_cell &k, character &ch);
    void draw_world(world &w, behaviour &b);
    void draw_world(world &w);
    void draw_world(world &w, behaviour &b, character &ch);
};

#endif
