#ifndef _BORDER_GENERATOR_HPP_
#define _BORDER_GENERATOR_HPP_

#include "world/generator.hpp"
#include "world/game_cell.hpp"

class border_generator : public generator<game_cell> {
    public:
    void generate(world<game_cell> &w);
};

#endif
