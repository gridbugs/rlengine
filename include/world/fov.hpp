#ifndef _FOV_HPP_
#define _FOV_HPP_

#include "world/grid.hpp"
#include "world/game_cell.hpp"
#include "actor/knowledge.hpp"
#include "geometry/vec2.hpp"

#include <functional>
#include <vector>

class fov_detector {
    protected:

    grid<game_cell> &game_grid_;
    grid<generic_cell<bool>> visibility_cache_;

    void compute_octant_fov(const vec2<int> &eye_coord,
                            const vec2<double> &eye_centre,
                            double max_slope,
                            double min_slope,
                            direction::ordinal::direction_t transparent_corner,
                            direction::ordinal::direction_t opaque_corner,
                            const int lateral_max,
                            const int depth_max,
                            const int depth_direction,
                            const unsigned int lateral_index,
                            const std::function<void(game_cell&)> &fn);

    void compute_fov(const vec2<int> &eye_coord, 
                     const std::function<void(game_cell&)> &fn);

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
