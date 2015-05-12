#include "world/fov.hpp"
#include "util/arith.hpp"

static double compute_slope(const vec2<double> &v, const vec2<double> &eye,
                            const unsigned int lateral_axis, const unsigned int depth_axis) {
    return (v[lateral_axis] - eye[lateral_axis]) /
           ((v[depth_axis] - eye[depth_axis]) * -1);
}

static void compute_octant_fov(grid<game_cell> &grid,
                               const std::function<void(game_cell&)> &fn, 
                               const vec2<int> &eye_coord,
                               const vec2<double> &eye_position,
                               const int start_offset,
                               const int max_offset,
                               double min_slope,
                               double max_slope) {


    vec2<int> coord;
    const unsigned int lateral_axis = vec2<int>::X_IDX;
    const unsigned int depth_axis = 1 - lateral_axis;
    for (int i = start_offset; i <= max_offset; ++i) {

        coord[depth_axis] = eye_coord[depth_axis] - i;

        /* Range to scan for visible cells. A cell is visible if there is a direct
         * line of site from the eye position (centre of cell containing eye) to the
         * centre of that cell.
         */
        const double visible_min_scan = eye_position[lateral_axis] + (min_slope * i) + 0.4999999;
        const double visible_max_scan = eye_position[lateral_axis] + (max_slope * i) - 0.5;

        /* Total range to scan. This is different from the visible scan range. A line of
         * sight may pass through a cell without that cell being visible.
         */
        const double check_min_scan = eye_position[lateral_axis] + (min_slope * (i+0.5));
        const double check_max_scan = eye_position[lateral_axis] + (max_slope * (i+0.5));

        /* Convert the scan ranges into integer indices. */
        int visible_min_scan_idx = arithmetic::constrain(0, 
            static_cast<int>(std::floor(visible_min_scan)), grid.width);
        int visible_max_scan_idx = arithmetic::constrain(0, 
            static_cast<int>(std::floor(visible_max_scan)), grid.width);
        int check_min_scan_idx = arithmetic::constrain(0, 
            static_cast<int>(std::floor(check_min_scan)), grid.width);
        int check_max_scan_idx = arithmetic::constrain(0, 
            static_cast<int>(std::floor(check_max_scan)), grid.width);

        coord[lateral_axis] = visible_min_scan_idx;
        
        if (visible_min_scan_idx != check_min_scan_idx) {

        }
        
        while (true) {

            while (coord[lateral_axis] <= visible_max_scan_idx &&
                   !grid.get_cell(coord).is_opaque()) {
                
                fn(grid.get_cell(coord));
                ++coord[lateral_axis];
            }

            if (coord[lateral_axis] > visible_max_scan_idx) {
                break;
            }

            const vec2<double> &v = grid.get_cell(coord).corners[direction::southwest];
            (void)v;

        }

        if (visible_max_scan_idx != check_max_scan_idx) {

        }
    }
}

void fov_detector::compute_fov(const vec2<int> &eye_coord, 
                 const std::function<void(game_cell&)> &fn) {
    
//    game_cell &c = game_grid_.get_cell(eye_coord);
//    fn(c);
//    game_grid_.for_each_neighbour(c, fn);

    const vec2<double> &eye_position = game_grid_.get_cell(eye_coord).centre;

    compute_octant_fov(game_grid_ ,fn, eye_coord, eye_position, 1, eye_coord.y, -1, 0);
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

