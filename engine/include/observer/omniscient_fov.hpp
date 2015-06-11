#ifndef _OMNISCIENT_FOV_HPP_
#define _OMNISCIENT_FOV_HPP_

#include "observer/observer.hpp"

class omniscient_fov : public observer {
    public:
    void observe(const character &c, const grid<world_cell> &w_grid, grid<knowledge_cell> &k_grid) {
        k_grid.for_each([](knowledge_cell &k) {k.see();});
    }
};

#endif
