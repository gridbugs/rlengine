#include "world/fov.hpp"

static void unsee_all(grid<knowledge_cell> &k) {
    for (grid<knowledge_cell>::iterator it = k.begin(); it != k.end(); ++it) {
        it->unsee();
    }
}

static void octant_compute_fov( const vec2<int> &eye_coord,
                                const vec2<double> &eye_centre,
                                const grid<game_cell> &g, 
                                grid<knowledge_cell> &k,
                                double max_slope,
                                double min_slope,
                                direction::ordinal::direction_t transparent_corner,
                                direction::ordinal::direction_t opaque_corner,
                                const int lateral_max,
                                const int depth_max,
                                const int depth_direction,
                                const unsigned int lateral_index)
{

}

void fov::compute_fov(const vec2<int> &eye_coord, const grid<game_cell> &g, grid<knowledge_cell> &k) {
    unsee_all(k);
    
    knowledge_cell &kc = k.get_cell(eye_coord);
    for (grid<knowledge_cell>::neighbour_iterator it = k.neighbour_begin(kc); 
        it != k.neighbour_end(kc); ++it) {
        it->see();
    }
    kc.see();
    
    octant_compute_fov(eye_coord, g.get_cell(eye_coord).centre, g, k, 0, -1,
        direction::ordinal::southwest, direction::ordinal::northwest, 
        g.width, eye_coord.y, -1, vec2<int>::X_IDX);
}
