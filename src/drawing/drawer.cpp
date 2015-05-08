#include "drawing/drawer.hpp"
#include <ncurses.h>

void drawer::draw_grid(grid<game_cell> &g, const grid<knowledge_cell> &k) {
    for (int i = 0; i < g.height; ++i) {
        for (int j = 0; j < g.width; ++j) {
            draw_cell(g[i][j], k[i][j]);
        }
    }
}

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

void drawer::draw_world(world &w, behaviour &b) {
    draw_grid(w.map, b.get_knowledge_grid());
}
