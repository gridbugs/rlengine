#ifndef _FOV_HPP_
#define _FOV_HPP_

#include "world/grid.hpp"
#include "world/game_cell.hpp"
#include "actor/knowledge.hpp"
#include "geometry/vec2.hpp"

#include <vector>
#include <iostream>

class fov_detector {
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
    
    grid<game_cell> &game_grid_;
    grid<generic_cell<bool>> visibility_cache_;

    void compute_fov(const vec2<int> &eye_coord);

    void compute_octant_fov(   const game_cell &eye_cell,
                               const int depth_relative_index,
                               double min_slope,
                               double max_slope);

    void mark_cell_completely_visible(game_cell &c);
    void mark_cell_partially_visible(game_cell &c);
    std::vector<game_cell*> *current_vector_;

    public:
    fov_detector(grid<game_cell> &g) :
        game_grid_(g),
        visibility_cache_(g.width, g.height)
    {
        visibility_cache_.for_each([](generic_cell<bool> &c) { c.data = false; });       
    }

    void push_visible_cells(const vec2<int> &eye_coord, std::vector<game_cell*> &visible_cells);
};

#endif
