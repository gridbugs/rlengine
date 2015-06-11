#ifndef _BLIND_FOV_HPP_
#define _BLIND_FOV_HPP_

#include "observer/observer.hpp"

class blind_fov : public observer {

    public:
    void observe(const character &c, const grid<world_cell> &w_grid, grid<knowledge_cell> &k_grid) {}
};

#endif
