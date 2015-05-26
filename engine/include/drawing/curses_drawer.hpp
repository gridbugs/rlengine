#ifndef _CURSES_DRAWER_HPP_
#define _CURSES_DRAWER_HPP_
#include "drawing/drawer.hpp"

class curses_drawer : public drawer {
    public:
    curses_drawer();
    void draw_world_from_actor(world<game_cell> &w, const actor_drawing_interface &a);
    void draw_world(world<game_cell> &w);
    void draw_hud(world<game_cell> &w, const hud &h);
};

#endif
