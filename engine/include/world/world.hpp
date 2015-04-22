#ifndef _WORLD_HPP_
#define _WORLD_HPP_

#include "world/grid.hpp"
#include "world/game_cell.hpp"

class world {
    public:
    grid<game_cell> map;
    world(const int width, const int height) : map(width, height) {}
};

#endif
