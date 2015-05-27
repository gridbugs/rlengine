#ifndef _BORDER_GENERATOR_HPP_
#define _BORDER_GENERATOR_HPP_

#include "world/generator.hpp"
#include "samples/game_cell.hpp"

template <typename C, typename W> class border_generator : public generator<C, W> {
    public:
    void generate(world<C, W> &w) {

        grid<W> &map = w.maps[0];

        for (typename grid<W>::iterator it = map.begin();
            it != map.end(); ++it) {

            it->set_data(new game_floor());
        }

        for (typename grid<W>::border_iterator it = map.border_begin();
            it != map.border_end(); ++it) {

            it->set_data(new game_wall());
            
        }

    }
};

#endif
