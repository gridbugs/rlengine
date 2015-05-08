#ifndef _DRAWER_HPP
#define _DRAWER_HPP

#include "world/grid.hpp"
#include "world/game_cell.hpp"
#include "world/world.hpp"
#include "behaviour/behaviour.hpp"

class drawer {

    public:
    virtual void draw_grid(grid<game_cell> &g);
    virtual void draw_grid(grid<game_cell> &g, const grid<knowledge_cell> &k);
    virtual void draw_cell(game_cell &c) = 0;
    virtual void draw_cell(game_cell &c, knowledge_cell &k) = 0;
    virtual void draw_world(world &w, behaviour &b);
    virtual void draw_world(world &w);
};

#endif
