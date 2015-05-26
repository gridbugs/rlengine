#ifndef _OMNISCIENT_FOV_HPP_
#define _OMNISCIENT_FOV_HPP_

#include "fov/fov.hpp"
#include "world/grid.hpp"

class omniscient_fov : public fov {
    protected:
    grid_interface &game_grid_;
    public:
    omniscient_fov(grid_interface &g) :
        game_grid_(g)
    {}
    void push_visible_cells(const vec2<int> &eye_coord, std::vector<const vec2<int>*> &visible_cells);
};

#endif
