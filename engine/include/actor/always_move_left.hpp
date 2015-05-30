#ifndef _ALWAY_MOVE_LEFT_HPP_
#define _ALWAY_MOVE_LEFT_HPP_

#include "behaviour/behaviour.hpp"
#include "actor/character_actor.hpp"
#include "action/action.hpp"
#include <functional>

using namespace behaviour_tree;
using namespace action;
using namespace std::placeholders;

template <typename C, typename W, typename K> class always_move_left :
    public character_actor<C, W, K> {
    
    protected:
    typedef typename character_actor<C, W, K>::world_t world_t;

    int act(world_t &w) {
        timed_result r = get_context(seq() && std::bind(move<C, W>, std::ref(w), std::ref(this->character_), direction::west));
        return r.time;
    }
    bool can_act() const {return true;}

    public:
    always_move_left(C &c, world_t &w, observer<C, W, K> &o) :
        character_actor<C, W, K>(c, w, o)
    {}
};

#endif
