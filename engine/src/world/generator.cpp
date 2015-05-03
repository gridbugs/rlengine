#include "world/generator.hpp"
#include "world/game_cell.hpp"

void border_generator::generate(world &w) {

    for (grid<game_cell>::iterator it = w.map.begin();
        it != w.map.end(); ++it) {

        it->set_data(new game_floor());
    }

    for (grid<game_cell>::border_iterator it = w.map.border_begin();
        it != w.map.border_end(); ++it) {

        it->set_data(new game_wall());
        
    }

}