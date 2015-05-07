#ifndef _CURSES_DRAWER_HPP_
#define _CURSES_DRAWER_HPP_

#include "drawing/drawer.hpp"

class curses_drawer : public drawer {
    public:
    curses_drawer();
    void draw_cell(game_cell &c);
    void draw_cell(game_cell &c, knowledge_cell &k);
};

#endif
