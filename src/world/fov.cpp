#include <cmath>
#include <iostream>
using namespace std;
#include "world/fov.hpp"
#include "util/arith.hpp"

static double compute_slope(const vec2<double> &from, const vec2<double> &to) {
    return (to.x - from.x) / (to.y - from.y);
}


void fov_detector::compute_octant_fov(  const game_cell &eye_cell,
                                        double min_slope_initial,
                                        double max_slope_initial,

                                        const direction::ordinal::direction_t inner_direction,
                                        const direction::ordinal::direction_t outer_direction,
                                        const int depth_direction
                                        
                                    ) {

    recursion_stack_.push_back(stack_parameters(
        min_slope_initial, 
        max_slope_initial, 
        1
    ));

    while (!recursion_stack_.empty()) {
        double min_slope = recursion_stack_.back().min_slope;
        double max_slope = recursion_stack_.back().max_slope;
        int depth_relative_index = recursion_stack_.back().depth_relative_index; 
        
        recursion_stack_.pop_back();

        const int depth_absolute_index = eye_cell.coord.y + depth_relative_index * depth_direction;

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
                mark_cell_completely_visible(c);
            } else {
                mark_cell_partially_visible(c);
            }

            bool current_opaque = c.is_opaque();
            if (previous_opaque && !current_opaque) {
                /* First transparent cell in transparent strip. */
                min_slope = compute_slope(eye_cell.centre, c.corners[outer_direction]) * depth_direction;
            }
            
            if (current_opaque && !previous_opaque && !first_iteration) {
                /* First opaque cell in opaque strip. */
                const double new_max_slope = compute_slope(eye_cell.centre, c.corners[inner_direction]) * depth_direction;
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

void fov_detector::compute_fov(const vec2<int> &eye_coord) {
    
    game_cell &c = game_grid_.get_cell(eye_coord);
    compute_octant_fov(c, -1, 0, direction::ordinal::southwest, direction::ordinal::northwest, -1);
    compute_octant_fov(c,  0, 1, direction::ordinal::northwest, direction::ordinal::southwest, -1);
    compute_octant_fov(c,  -1, 0, direction::ordinal::northwest, direction::ordinal::southwest, 1);
    compute_octant_fov(c,  0, 1, direction::ordinal::southwest, direction::ordinal::northwest, 1);
}

void fov_detector::mark_cell_completely_visible(game_cell &c) {
        generic_cell<bool> &b = visibility_cache_.get_cell(c.coord);
        if (b.data == false) {
            b.data = true;
            current_vector_->push_back(&c);
        }
}
void fov_detector::mark_cell_partially_visible(game_cell &c) {
    mark_cell_completely_visible(c);
}

void fov_detector::push_visible_cells(const vec2<int> &eye_coord, std::vector<game_cell*> &visible_cells) {

    current_vector_ = &visible_cells;

    compute_fov(eye_coord);

    for (std::vector<game_cell*>::iterator it = current_vector_->begin();
        it != current_vector_->end(); ++it) {
        
        game_cell *c = *it;


        visibility_cache_.get_cell(c->coord).data = false;
    }
}

