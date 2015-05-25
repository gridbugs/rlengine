#ifndef _SHADOW_CAST_FOV_HPP_
#define _SHADOW_CAST_FOV_HPP_

#include "world/game_grid.hpp"
#include "geometry/vec2.hpp"
#include "fov/fov.hpp"

#include <vector>

class shadow_cast_fov : public fov {
    protected:
    
    typedef struct {
        double min_slope;
        double max_slope;
        int depth_relative_index;
    } fov_stack_parameters_t;
    
    class stack_parameters {
        public:
        double min_slope;
        double max_slope;
        int depth_relative_index;
        stack_parameters(const double &min_slope, const double &max_slope, const int &depth_relative_index) :
            min_slope(min_slope),
            max_slope(max_slope),
            depth_relative_index(depth_relative_index)
        {}
    };

    std::vector<stack_parameters> recursion_stack_;
    
//    grid<game_cell_interface> &game_grid_;
    game_grid_interface &game_grid_;
    grid<generic_cell<bool>> visibility_cache_;

    void compute_fov(const vec2<int> &eye_coord);

    void compute_octant_fov(   const game_cell_interface &eye_cell,
                               double min_slope,
                               double max_slope,
                               const direction::ordinal::direction_t inner_direction,
                               const direction::ordinal::direction_t outer_direction,
                               const int depth_direction,
                               const unsigned int lateral_index,
                               const int lateral_max
                            );

    void mark_cell_completely_visible(game_cell_interface &c);
    void mark_cell_partially_visible(game_cell_interface &c);
    std::vector<game_cell_interface*> *current_vector_;

    public:
    shadow_cast_fov(game_grid_interface &g) :
        game_grid_(g),
        visibility_cache_(g.get_width(), g.get_height())
    {
        visibility_cache_.for_each([](generic_cell<bool> &c) { c.data = false; });       
    }

    void push_visible_cells(const vec2<int> &eye_coord, std::vector<game_cell_interface*> &visible_cells);
};

#endif
