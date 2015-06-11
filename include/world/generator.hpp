#ifndef _GENERATOR_HPP_
#define _GENERATOR_HPP_

#include "world/world.hpp"

class generator {
    public:
    virtual void generate(world &w) = 0;
};

#endif
