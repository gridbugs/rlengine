#include "world/fov.hpp"

void fov_detector::compute_octant_fov(const vec2<int> &eye_coord,
                        const vec2<double> &eye_centre,
                        double max_slope,
                        double min_slope,
                        direction::ordinal::direction_t transparent_corner,
                        direction::ordinal::direction_t opaque_corner,
                        const int lateral_max,
                        const int depth_max,
                        const int depth_direction,
                        const unsigned int lateral_index,
                        const std::function<void(game_cell&)> &fn) {

}

void fov_detector::compute_fov(const vec2<int> &eye_coord, 
                 const std::function<void(game_cell&)> &fn) {
    
    game_cell &c = game_grid_.get_cell(eye_coord);
    fn(c);
    game_grid_.for_each_neighbour(c, fn);

    compute_octant_fov(eye_coord, game_grid_.get_cell(eye_coord).centre,
        0, -1, direction::ordinal::southwest, direction::ordinal::northwest,
        game_grid_.width, eye_coord.y, -1, vec2<int>::X_IDX, fn);
}


void fov_detector::push_visible_cells(const vec2<int> &eye_coord, std::vector<game_cell*> &visible_cells) {
    grid<generic_cell<bool>> &visibility_cache = visibility_cache_;
    compute_fov(eye_coord, [&visible_cells, &visibility_cache](game_cell &c) {
        generic_cell<bool> &b = visibility_cache.get_cell(c.coord);
        if (b.data == false) {
            b.data = true;
            visible_cells.push_back(&c);
        }
    });

    for (std::vector<game_cell*>::iterator it = visible_cells.begin();
        it != visible_cells.end(); ++it) {
        
        game_cell *c = *it;
        visibility_cache.get_cell(c->coord).data = false;
    }
}

