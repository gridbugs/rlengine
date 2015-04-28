#ifndef _CHARACTER_HPP_
#define _CHARACTER_HPP_

#include "geometry/vec2.hpp"
#include "actor/actor.hpp"

class character : public actor {
    public:
    vec2<int> position;
    character(int x, int y) : 
        position(x, y)
    {}
    character(vec2<int> v) :
        position(v)
    {}
};

#endif
