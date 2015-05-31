#ifndef _CHARACTER_HPP_
#define _CHARACTER_HPP_

#include "geometry/vec2.hpp"
#include "geometry/direction.hpp"

class character {
    public:
    vec2<int> coord;
    int level_index = 0;

    character(const vec2<int> &v) :
        coord(v)
    {}

    virtual void take_damage(int d) = 0;
    virtual int get_move_time(direction::direction_t d) const = 0;
    virtual int get_melee_range() const = 0;
};

#endif
