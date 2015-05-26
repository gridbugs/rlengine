#ifndef _BLIND_FOV_HPP_
#define _BLIND_FOV_HPP_

#include "fov/fov.hpp"

class blind_fov : public fov {
    public:
    void push_visible_cells(const vec2<int> &eye_coord, std::vector<const vec2<int>*> &visible_cells) {}
};

#endif
