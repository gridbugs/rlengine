#ifndef _GENERATOR_HPP_
#define _GENERATOR_HPP_

#include "world/world.hpp"

template <typename W> class generator {
    public:
    virtual void generate(world<W> &w) = 0;
};

#endif
