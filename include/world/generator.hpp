#ifndef _GENERATOR_HPP_
#define _GENERATOR_HPP_

#include "world/world.hpp"

class generator {
    public:
    generator(){}
    virtual void generate(world &w) = 0;
    virtual ~generator(){}
};

class border_generator : public generator {
    public:
    border_generator() : generator()
    {}
    void generate(world &w);

    ~border_generator(){}
};

#endif
