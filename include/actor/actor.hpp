#ifndef _ACTOR_HPP_
#define _ACTOR_HPP_

#include "world/world.hpp"
#include "action/action.hpp"

class actor {
    public:
    virtual action& get_action(world &w) = 0;
    virtual bool can_act() = 0;
};

#endif
