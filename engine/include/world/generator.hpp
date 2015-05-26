#ifndef _GENERATOR_HPP_
#define _GENERATOR_HPP_

#include "world/world.hpp"

template <typename C, typename W> class generator {
    public:
    virtual void generate(world<C, W> &w) = 0;
};

#endif
