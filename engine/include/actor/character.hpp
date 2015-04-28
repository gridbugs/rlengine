#ifndef _CHARACTER_HPP_
#define _CHARACTER_HPP_

#include "geometry/vec2.hpp"
#include "actor/actor.hpp"
#include "behaviour/behaviour.hpp"

class character : public actor {
    protected:
    behaviour *behaviour_;

    public:
    vec2<int> position;

    character(behaviour &b, int x, int y) :
        position(x, y)
    {
        set_behaviour(b);
    }

    character(behaviour &b, vec2<int> v) : 
        position(v)
    {
        set_behaviour(b);
    }

    character(int x, int y) :
        behaviour_(nullptr),
        position(x, y)
    {}
    character(vec2<int> v) :
        behaviour_(nullptr),
        position(v)
    {}

    bool set_behaviour(behaviour &b);
    bool has_behaviour() {return behaviour_ != nullptr;}

    action& get_action(world &w) {
        return behaviour_->get_action(w);
    }
    bool can_act() {
        return behaviour_->can_act();
    }
};

#endif
