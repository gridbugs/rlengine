#include "world/world.hpp"


game_cell& world::get_random_empty_cell() {
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
    
void world::move_character(character &c, vec2<int> coord) {
    if (!map.get_cell(coord).is_solid()) {
        c.position = coord;
    }
}
