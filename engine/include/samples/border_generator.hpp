#ifndef _BORDER_GENERATOR_HPP_
#define _BORDER_GENERATOR_HPP_

#include "world/generator.hpp"
#include "samples/game_cell.hpp"
#include "character/character.hpp"

class border_generator : public generator<character, game_cell> {
    public:
    void generate(world<character, game_cell> &w);
};

#endif
