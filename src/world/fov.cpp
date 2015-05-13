#include <cmath>
#include <iostream>
using namespace std;
#include "world/fov.hpp"
#include "util/arith.hpp"

static double compute_slope(const vec2<double> &from, const vec2<double> &to) {
    return (to.x - from.x) / (to.y - from.y);
}


void fov_detector::compute_octant_fov(
                               const std::function<void(game_cell&)> &completely_visible_fn, 
                               const std::function<void(game_cell&)> &partially_visible_fn, 
                               const game_cell &eye_cell,
                               const int depth_relative_index_initial,
                               double min_slope_initial,
                               double max_slope_initial) {

    recursion_stack_.push_back(stack_parameters(
        min_slope_initial, 
        max_slope_initial, 
        depth_relative_index_initial
    ));

    while (!recursion_stack_.empty()) {
        double min_slope = recursion_stack_.back().min_slope;
        double max_slope = recursion_stack_.back().max_slope;
        int depth_relative_index = recursion_stack_.back().depth_relative_index; 
        
        recursion_stack_.pop_back();

        const int depth_absolute_index = eye_cell.coord.y - depth_relative_index;

        if (depth_absolute_index < 0) {
            continue;
        }
        
        const double inner_depth_offset = depth_relative_index - 0.5;
        const double outer_depth_offset = inner_depth_offset + 1;
        const double min_inner_lateral_offset = min_slope * inner_depth_offset;
        const double min_outer_lateral_offset = min_slope * outer_depth_offset;
        const int partial_start_index = std::floor(min_outer_lateral_offset + eye_cell.centre.x);
        const int complete_start_index = std::ceil(min_inner_lateral_offset + eye_cell.centre.x);
        const double max_inner_lateral_offset = max_slope * inner_depth_offset;
        const double max_outer_lateral_offset = max_slope * outer_depth_offset;
        const int partial_stop_index = std::floor(max_outer_lateral_offset + eye_cell.centre.x);
        const int complete_stop_index = std::floor(max_inner_lateral_offset + eye_cell.centre.x) - 1;
        
        const int start_index = arithmetic::constrain(0, partial_start_index, game_grid_.width);
        const int stop_index = arithmetic::constrain(0, partial_stop_index, game_grid_.width);

        bool first_iteration = true;
        bool previous_opaque = false;
        for (int i = start_index; i <= stop_index; ++i) {
            const bool last_iteration = i == partial_stop_index;

            game_cell &c = game_grid_[depth_absolute_index][i];
            if (i >= complete_start_index && i <= complete_stop_index) {
                completely_visible_fn(c);
            } else {
                partially_visible_fn(c);
            }

            bool current_opaque = c.is_opaque();
            if (previous_opaque && !current_opaque) {
                /* First transparent cell in transparent strip. */
                min_slope = -compute_slope(eye_cell.centre, c.corners[direction::ordinal::northwest]);
            }
            
            if (current_opaque && !previous_opaque && !first_iteration) {
                /* First opaque cell in opaque strip. */
                const double new_max_slope = -compute_slope(eye_cell.centre, c.corners[direction::ordinal::southwest]);
                recursion_stack_.push_back(stack_parameters(
                    min_slope,
                    new_max_slope,
                    depth_relative_index + 1
                ));
            }

            if (!current_opaque && last_iteration) {
                /* Last cell in strip and it happens to be transparent. */
                recursion_stack_.push_back(stack_parameters(
                    min_slope,
                    max_slope,
                    depth_relative_index + 1
                ));
            }

            previous_opaque = current_opaque;
            first_iteration = false;

        }
    }
}

void fov_detector::compute_fov(const vec2<int> &eye_coord, 
                 const std::function<void(game_cell&)> &completely_visible_fn, 
                 const std::function<void(game_cell&)> &partially_visible_fn) {
    
    compute_octant_fov(completely_visible_fn, partially_visible_fn, game_grid_.get_cell(eye_coord), 1, -1, 0);
}


void fov_detector::push_visible_cells(const vec2<int> &eye_coord, std::vector<game_cell*> &visible_cells) {
    grid<generic_cell<bool>> &visibility_cache = visibility_cache_;

    std::function<void(game_cell&)> fn = [&visible_cells, &visibility_cache](game_cell &c) {

        generic_cell<bool> &b = visibility_cache.get_cell(c.coord);
        if (b.data == false) {
            b.data = true;
            visible_cells.push_back(&c);
        }
    };

    compute_fov(eye_coord, fn, fn);

    for (std::vector<game_cell*>::iterator it = visible_cells.begin();
        it != visible_cells.end(); ++it) {
        
        game_cell *c = *it;


        visibility_cache.get_cell(c->coord).data = false;
    }
}

