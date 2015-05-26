#ifndef _BORDER_GENERATOR_HPP_
#define _BORDER_GENERATOR_HPP_

#include "world/generator.hpp"

class border_generator : public generator {
    public:
    border_generator() : generator()
    {}
    generator* generate(world<game_cell> &w);

    ~border_generator(){}
};

#endif
