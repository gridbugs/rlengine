#ifndef _CHARACTER_HPP_
#define _CHARACTER_HPP_

#include "geometry/vec2.hpp"
#include "actor/actor.hpp"
#include "behaviour/behaviour.hpp"
#include "actor/character_statistics.hpp"

class character : public actor, public character_statistics {
    protected:
    behaviour *behaviour_;

    public:
    vec2<int> position;

    character(int x, int y) :
        behaviour_(nullptr),
        position(x, y)
    {}
    character(const vec2<int> &v) :
        behaviour_(nullptr),
        position(v)
    {}

    void set_behaviour(behaviour &b);
    bool has_behaviour() {return behaviour_ != nullptr;}

    action& get_action(const world &w) {
        return behaviour_->get_action(w);
    }
    bool can_act() {
        return behaviour_->can_act();
    }
    void observe_world(const world &w) {
        behaviour_->observe_world(w);
    }

    int get_agility() {return 1;}
};

#endif
