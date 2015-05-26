#ifndef _CURSES_DRAWER_HPP_
#define _CURSES_DRAWER_HPP_
#include "drawing/drawer.hpp"
#include "samples/game_cell.hpp"
#include "samples/knowledge.hpp"

class curses_drawer : public drawer<game_cell, knowledge_cell> {
    public:
    curses_drawer();
    void draw_world_from_actor(world<game_cell> &w, const actor_drawing_interface<knowledge_cell> &a);
};

#endif
