#ifndef _CONWAY_GENERATOR_HPP_
#define _CONWAY_GENERATOR_HPP_

#include "world/generator.hpp"
#include "world/game_cell.hpp"

class conway_generator : public generator<game_cell> {
    
    private:
    class conway_cell;
    class conway_grid;
    int generate_attempt(conway_grid &cg);

    public:
    void generate(world<game_cell> &w);
};

#endif
