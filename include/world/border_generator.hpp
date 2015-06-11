#ifndef _BORDER_GENERATOR_HPP_
#define _BORDER_GENERATOR_HPP_

#include "world/generator.hpp"

class border_generator : public generator {
    public:
    void generate(world &w) {

        grid<world_cell> &map = w.maps[0];

        for (grid<world_cell>::iterator it = map.begin();
            it != map.end(); ++it) {

            it->set_data(new game_floor());
        }

        for (grid<world_cell>::border_iterator it = map.border_begin();
            it != map.border_end(); ++it) {

            it->set_data(new game_wall());
            
        }

    }
};

#endif
