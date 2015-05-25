#ifndef _OMNISCIENT_FOV_HPP_
#define _OMNISCIENT_FOV_HPP_

#include "fov/fov.hpp"
#include "world/grid.hpp"
#include "world/game_cell.hpp"

class omniscient_fov : public fov {
    protected:
    grid<game_cell> &game_grid_;
    public:
    omniscient_fov(grid<game_cell> &g) :
        game_grid_(g)
    {}
    void push_visible_cells(const vec2<int> &eye_coord, std::vector<game_cell*> &visible_cells);
};

#endif
