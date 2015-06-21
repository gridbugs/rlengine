#ifndef _CELL_HPP_
#define _CELL_HPP_

#include <iostream>
#include "geometry/vec2.hpp"
#include "geometry/direction.hpp"

class cell {
    public:
    const vec2<int> coord;
    const int x_coord;
    const int y_coord;

    const vec2<double> centre;
    const std::array<vec2<double>, direction::n_ordinal_directions> corners;


    cell(const int j, const int i) :
        coord(j, i),
        x_coord(j), y_coord(i),
        centre(j+0.5, i+0.5),
        corners({{
            /* northeast */ vec2<double>(j+1, i),
            /* southeast */ vec2<double>(j+1, i+1),
            /* southwest */ vec2<double>(j, i+1),
            /* northwest */ vec2<double>(j, i)
        }})
    {

    }
};

template <typename T> class generic_cell : public cell {
    public:
    T data;
    generic_cell(const int j, const int i) : cell(j, i) {};
};


template <typename T>
std::ostream& operator<<(std::ostream &out, const generic_cell<T> &c) {
    return out << c.data;
}

#endif
