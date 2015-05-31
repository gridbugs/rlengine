#ifndef _ALWAY_MOVE_LEFT_HPP_
#define _ALWAY_MOVE_LEFT_HPP_

#include "behaviour/behaviour.hpp"
#include "actor/character_actor.hpp"
#include "action/action.hpp"
#include <functional>

using namespace behaviour_tree;

template <typename C, typename W, typename K> class always_move_left :
    public character_actor<C, W, K> {
    
    protected:
    typedef typename character_actor<C, W, K>::world_t world_t;

    int act(world_t &w) {
        timed_result r = seq() && move_in_direction_action<C, W>(w, this->character_, direction::west);
        return r.time;
    }
    bool can_act() const {return true;}

    public:
    always_move_left(C &c, world_t &w, observer<C, W, K> &o) :
        character_actor<C, W, K>(c, w, o)
    {}
};

#endif
