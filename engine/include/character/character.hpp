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

    virtual void take_damage(int d) {}
    virtual int get_move_time() {return 1;}
    virtual int get_melee_range() {return 1;}
};

#endif
