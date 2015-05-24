#ifndef _WORLD_HPP_
#define _WORLD_HPP_

#include "world/grid.hpp"
#include "world/game_cell.hpp"
#include "geometry/vec2.hpp"
#include "character/character.hpp"

class world {
    public:
    grid<game_cell> map;
    const int width;
    const int height;
    world(const int width, const int height) : 
        map(width, height),
        width(width),
        height(height)
    {}

    game_cell& get_random_empty_cell();   

    void move_character(character &c, vec2<int> coord);

};

#endif
