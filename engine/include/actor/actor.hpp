#ifndef _ACTOR_HPP_
#define _ACTOR_HPP_

#include "character/character.hpp"
#include "schedule/schedule.hpp"

class actor : public schedule_callback {};

class character_actor : public actor {
    protected:
    character &character_;
    virtual int act(world &w) = 0;
    virtual bool can_act() const = 0;
    
    public:
    character_actor(character &c) : character_(c) {}

    void operator()(world& w, callback_registry& cr) {
        if (can_act()) {
            int cooldown = act(w);
            if (can_act()) {
                cr.register_callback(*this, cooldown);
            }
        }
    }
};


#endif
