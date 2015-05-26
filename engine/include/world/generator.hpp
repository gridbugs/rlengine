#ifndef _GENERATOR_HPP_
#define _GENERATOR_HPP_

#include "world/world.hpp"

class generator {
    public:
    generator(){}
    virtual generator* generate(world<game_cell> &w) = 0;
    virtual ~generator(){}
};

class border_generator : public generator {
    public:
    border_generator() : generator()
    {}
    generator* generate(world<game_cell> &w);

    ~border_generator(){}
};

#endif
