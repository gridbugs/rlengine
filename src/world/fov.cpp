#include <cmath>
#include <iostream>
using namespace std;
#include "world/fov.hpp"
#include "util/arith.hpp"

static inline double compute_slope(const vec2<double> &from, const vec2<double> &to, 
                            unsigned int lateral_index, unsigned int depth_index) {
    return (to[lateral_index] - from[lateral_index]) / (to[depth_index] - from[depth_index]);
}


void fov_detector::compute_octant_fov(  const game_cell &eye_cell,
                                        double min_slope_initial,
                                        double max_slope_initial,
                                        const direction::ordinal::direction_t inner_direction,
                                        const direction::ordinal::direction_t outer_direction,
                                        const int depth_direction,
                                        const unsigned int lateral_index,
                                        const int lateral_max
                                    ) {

    unsigned int depth_index = vec2<>::get_other_index(lateral_index);

    recursion_stack_.push_back(stack_parameters(
        min_slope_initial, 
        max_slope_initial, 
        1
    ));
    int depth = 0;
    while (!recursion_stack_.empty()) {
        double min_slope = recursion_stack_.back().min_slope;
        double max_slope = recursion_stack_.back().max_slope;
        int depth_relative_index = recursion_stack_.back().depth_relative_index; 
        recursion_stack_.pop_back();

        const int depth_absolute_index = eye_cell.coord[depth_index] + depth_relative_index * depth_direction;

        if (depth_absolute_index < 0) {
            continue;
        }
        
        const double inner_depth_offset = depth_relative_index - 0.5;
        const double outer_depth_offset = inner_depth_offset + 1;

        const double min_inner_lateral_offset = min_slope * inner_depth_offset;
        const double min_outer_lateral_offset = min_slope * outer_depth_offset;
        
        const double min_inner_lateral_position = min_inner_lateral_offset + eye_cell.centre[lateral_index];
        const double min_outer_lateral_position = min_outer_lateral_offset + eye_cell.centre[lateral_index];

        const int partial_start_index = std::floor(std::min(min_inner_lateral_position, min_outer_lateral_position));
        const int complete_start_index = std::ceil(std::max(min_inner_lateral_position, min_outer_lateral_position));
        
        const double max_inner_lateral_offset = max_slope * inner_depth_offset;
        const double max_outer_lateral_offset = max_slope * outer_depth_offset;
        
        const double max_inner_lateral_position = max_inner_lateral_offset + eye_cell.centre[lateral_index] - 1;
        const double max_outer_lateral_position = max_outer_lateral_offset + eye_cell.centre[lateral_index] - 1;

        const int partial_stop_index = std::ceil(std::max(max_inner_lateral_position, max_outer_lateral_position));
        const int complete_stop_index = std::floor(std::min(max_inner_lateral_position, max_outer_lateral_position));
        
        const int start_index = arithmetic::constrain(0, partial_start_index, lateral_max);
        const int stop_index = arithmetic::constrain(0, partial_stop_index, lateral_max);
        
        bool first_iteration = true;
        bool previous_opaque = false;
        vec2<int> coord_idx;
        coord_idx[depth_index] = depth_absolute_index;
        for (int i = start_index; i <= stop_index; ++i) {
            const bool last_iteration = i == partial_stop_index;
            
            coord_idx[lateral_index] = i;
            game_cell &c = game_grid_.get_cell(coord_idx);

            if (i >= complete_start_index && i <= complete_stop_index) {
                mark_cell_completely_visible(c);
            } else {
                mark_cell_partially_visible(c);
            }

            bool current_opaque = c.is_opaque();
            if (previous_opaque && !current_opaque) {
                /* First transparent cell in transparent strip. */
                min_slope = compute_slope(eye_cell.centre, c.corners[outer_direction], 
                                lateral_index, depth_index) * depth_direction;
            }
            
            if (current_opaque && !previous_opaque && !first_iteration) {
                /* First opaque cell in opaque strip. */
                const double new_max_slope = compute_slope(eye_cell.centre, c.corners[inner_direction],
                                                lateral_index, depth_index) * depth_direction;
//                cout << "recurse (solid start)" << depth << " " << min_slope << " " << new_max_slope << c.coord << endl;
                recursion_stack_.push_back(stack_parameters(
                    min_slope,
                    new_max_slope,
                    depth_relative_index + 1
                ));
            }

            if (!current_opaque && last_iteration) {
//                cout << "recurse (last iteration)" << depth << " " << min_slope << " " << max_slope << c.coord << endl;
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

        ++depth;
        /*
        if (depth == 4) {
            recursion_stack_.clear();
            return;
        }
        */
    }
}

void fov_detector::compute_fov(const vec2<int> &eye_coord) {
    
    int width = game_grid_.width;
    int height = game_grid_.height;

    game_cell &c = game_grid_.get_cell(eye_coord);
#if 1
    compute_octant_fov(c, -1, 0, direction::ordinal::southwest, direction::ordinal::northwest, -1, vec2<>::X_IDX, width);
    compute_octant_fov(c,  0, 1, direction::ordinal::northwest, direction::ordinal::southwest, -1, vec2<>::X_IDX, width);
    compute_octant_fov(c,  -1, 0, direction::ordinal::northwest, direction::ordinal::southwest, 1, vec2<>::X_IDX, width);
    compute_octant_fov(c,  0, 1, direction::ordinal::southwest, direction::ordinal::northwest, 1, vec2<>::X_IDX, width);
#endif
    compute_octant_fov(c,  -1, 0, direction::ordinal::northeast, direction::ordinal::northwest, -1, vec2<>::Y_IDX, height);
    compute_octant_fov(c,  0, 1, direction::ordinal::northwest, direction::ordinal::northeast, -1, vec2<>::Y_IDX, height);
    compute_octant_fov(c,  -1, 0, direction::ordinal::northwest, direction::ordinal::northeast, 1, vec2<>::Y_IDX, height);
    compute_octant_fov(c,  0, 1, direction::ordinal::northeast, direction::ordinal::northwest, 1, vec2<>::Y_IDX, height);
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

