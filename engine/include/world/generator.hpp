#ifndef _GENERATOR_HPP_
#define _GENERATOR_HPP_

#include "world/world.hpp"

class generator {
    public:
    generator(){}
    virtual generator* generate(world<game_cell> &w) = 0;
    virtual ~generator(){}
};

#endif
