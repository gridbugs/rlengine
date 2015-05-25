#ifndef _STATIONARY_ACTOR_HPP_
#define _STATIONARY_ACTOR_HPP_

#include "actor/character_actor.hpp"

class stationary_actor : public character_actor {
    public:
    stationary_actor(character &c, world &w, fov &f) :
        character_actor(c, w, f)
    {}

    bool can_act() const {return true;}
    int act(world &w) {}
};

#endif
