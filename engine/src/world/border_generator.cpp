#include "world/border_generator.hpp"
#include "world/game_cell.hpp"

generator* border_generator::generate(world<game_cell> &w) {

    grid<game_cell> &map = w.maps[0];

    for (grid<game_cell>::iterator it = map.begin();
        it != map.end(); ++it) {

        it->set_data(new game_floor());
    }

    for (grid<game_cell>::border_iterator it = map.border_begin();
        it != map.border_end(); ++it) {

        it->set_data(new game_wall());
        
    }

    return this;
}
