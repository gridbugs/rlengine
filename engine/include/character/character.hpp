#ifndef _CHARACTER_HPP_
#define _CHARACTER_HPP_

#include "geometry/vec2.hpp"

class character {
    public:
    vec2<int> coord;
    int level_index = 0;

    character(const vec2<int> &v) :
        coord(v)
    {}

    virtual void take_damage(int d) = 0;
};

#endif
