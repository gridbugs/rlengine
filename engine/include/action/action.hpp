#ifndef _ACTION_HPP_
#define _ACTION_HPP_

#include "behaviour/behaviour.hpp"
#include "geometry/direction.hpp"
#include "debug/fifo.hpp"

namespace action {

    template <typename C, typename W> timed_result move(world<C, W> &w, C &c, direction::direction_t d) {
        grid<W> &map = w.maps[c.level_index];
        W *cell_ptr = map.get_neighbour(map.get_cell(c.coord), d);
        if (cell_ptr != nullptr && !cell_ptr->is_solid()) {
            fifo::cout << "moving " << c.coord << " to " << cell_ptr->coord << fifo::endl;
            c.coord = cell_ptr->coord;
            fifo::cout << "done " << c.coord << fifo::endl;
            return timed_result(true, c.get_move_time(d));
        }
        return timed_result(false, c.get_move_time(d));
    }

};

#endif
