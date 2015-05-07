#ifndef _FOV_HPP_
#define _FOV_HPP_

#include "world/grid.hpp"
#include "world/game_cell.hpp"
#include "actor/knowledge.hpp"
#include "geometry/vec2.hpp"

namespace fov {
    void compute_fov(const vec2<int> &eye_coord, const grid<game_cell> &g, grid<knowledge_cell> &k);
};

#endif
