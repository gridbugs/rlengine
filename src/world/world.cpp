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
