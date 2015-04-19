#ifndef _CELL_HPP_
#define _CELL_HPP_

#include "geometry/vec2.hpp"
#include "geometry/directions.hpp"

class cell {
    public:
    const int x_coord;
    const int y_coord;

    const Vec2<double> centre;
    const std::array<Vec2<double>, directions::n_ordinal_directions> corners;


    cell(const int j, const int i) :
        x_coord(j), y_coord(i),
        centre(j+0.5, i+0.5),
        corners({{
            /* northeast */ Vec2<double>(j+1, i),
            /* southeast */ Vec2<double>(j+1, i+1),
            /* southwest */ Vec2<double>(j, i+1),
            /* northwest */ Vec2<double>(j, i)
        }})
    {

    }
};

#endif
