#ifndef _CONWAY_GENERATOR_HPP_
#define _CONWAY_GENERATOR_HPP_

#include "world/generator.hpp"

class conway_generator : public generator {
    
    private:
    class conway_cell;
    class conway_grid;
    int generate_attempt(conway_grid &cg);

    public:
    conway_generator() : generator() {}
    ~conway_generator(){}
    void generate(world &w);
};

#endif
