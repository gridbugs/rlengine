#ifndef _WORLD_HPP_
#define _WORLD_HPP_

#include "world/grid.hpp"
#include "world/game_cell.hpp"
#include "action/action_schedule.hpp"

class world {
    public:
    grid<game_cell> map;
    action_schedule schedule;
    world(const int width, const int height) : map(width, height) {}
};

#endif
