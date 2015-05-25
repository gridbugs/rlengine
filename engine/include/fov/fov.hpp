#ifndef _FOV_HPP_
#define _FOV_HPP_

#include "geometry/vec2.hpp"
#include "world/game_cell.hpp"
#include <vector>

class fov {
    public:
    virtual void push_visible_cells(const vec2<int> &eye_coord, std::vector<game_cell*> &visible_cells) = 0;
};

#endif
