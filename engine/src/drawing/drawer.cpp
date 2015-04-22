#include "drawing/drawer.hpp"

void drawer::draw_grid(grid<game_cell> &g) {
    for (int i = 0; i < g.height; ++i) {
        for (int j = 0; j < g.width; ++j) {
            draw_cell(g[i][j]);
        }
    }
}

void drawer::draw_world(world &w) {
    draw_grid(w.map);
}
