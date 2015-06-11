#ifndef _OBSERVER_HPP_
#define _OBSERVER_HPP_

#include "world/grid.hpp"

class observer {
    public:
    virtual void observe(const character &c, const grid<world_cell> &w_grid, grid<knowledge_cell> &k_grid) = 0;
};

#endif
