#include "fov/omniscient_fov.hpp"

void omniscient_fov::push_visible_cells(const vec2<int> &eye_coord, std::vector<game_cell*> &visible_cells) {
    game_grid_.for_each([&](game_cell &c) {
        visible_cells.push_back(&c);
    });
}
