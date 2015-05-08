#ifndef _WORLD_HPP_
#define _WORLD_HPP_

#include "world/grid.hpp"
#include "world/game_cell.hpp"
#include "action/action_schedule.hpp"
#include "geometry/vec2.hpp"

class world {
    public:
    grid<game_cell> map;
    action_schedule schedule;
    const int width;
    const int height;
    world(const int width, const int height) : 
        map(width, height),
        schedule(*this),
        width(width),
        height(height)
    {}

    game_cell& get_random_empty_cell();   

};

#endif
