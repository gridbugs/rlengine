#ifndef _WORLD_HPP_
#define _WORLD_HPP_

#include "world/grid.hpp"
#include "world/game_cell.hpp"
#include "geometry/vec2.hpp"
#include "character/character.hpp"

template <typename W> class world {
    public:
    grid<W> map;
    const int width;
    const int height;
    world(const int width, const int height) : 
        map(width, height),
        width(width),
        height(height)
    {}

    W& get_random_empty_cell() {
        for (;;) {
            int x = rand() % width;
            int y = rand() % height;
            game_cell& ret = map.get_cell(x, y);
            if (!ret.is_solid()) {
                return ret;
            }
        }
        return map[0][0];
    }

    void move_character(character &c, vec2<int> coord) {
        if (!map.get_cell(coord).is_solid()) {
            c.position = coord;
        }
    }
};

#endif
