#ifndef _OMNISCIENT_FOV_HPP_
#define _OMNISCIENT_FOV_HPP_

#include "fov/fov.hpp"
#include "world/grid.hpp"
#include "world/game_grid.hpp"

class omniscient_fov : public fov {
    protected:
    game_grid_interface &game_grid_;
    public:
    omniscient_fov(game_grid_interface &g) :
        game_grid_(g)
    {}
    void push_visible_cells(const vec2<int> &eye_coord, std::vector<game_cell_interface*> &visible_cells);
};

#endif
