#include "fov/omniscient_fov.hpp"

void omniscient_fov::push_visible_cells(const vec2<int> &eye_coord, std::vector<const vec2<int>*> &visible_cells) {
    game_grid_.for_each_cell([&](cell &c) {
        visible_cells.push_back(&c.coord);
    });
}
