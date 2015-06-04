#ifndef _GENERATOR_HPP_
#define _GENERATOR_HPP_

#include "world/world.hpp"

template <typename C, typename W, typename T> class generator {
    public:
    virtual void generate(world<C, W, T> &w) = 0;
};

#endif
