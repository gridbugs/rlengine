#ifndef _PLAYER_ACTOR_HPP_
#define _PLAYER_ACTOR_HPP_

#include <array>
#include <functional>
#include "actor/actor.hpp"
#include "geometry/direction.hpp"
#include "drawing/drawer.hpp"
#include "ui/hud.hpp"

class player_actor : public character_actor {

    protected:
    int act(world &w);
    bool can_act() const {return true;}
    drawer &drawer_; // XXX separate this into a different class
    hud &hud_;

    class action {
        public:
        virtual int operator()(character &c, world &w) = 0;
    };

    class move_action : public action {
        private:
        direction::direction_t direction_;
        public:
        move_action(direction::direction_t d) : direction_(d) {}
        int operator()(character &c, world &w);
    };

    static const unsigned int N_KEY_BINDINGS = 512;
    std::array<action*, N_KEY_BINDINGS> key_bindings_;

    move_action move_north_;
    move_action move_south_;
    move_action move_east_;
    move_action move_west_;
    move_action move_northeast_;
    move_action move_northwest_;
    move_action move_southeast_;
    move_action move_southwest_;

    public:
    
    player_actor(character &c, world &w, fov_detector &f, drawer &d, hud &h) :
        character_actor(c, w, f),
        drawer_(d),
        hud_(h),
        move_north_(direction::north),
        move_south_(direction::south),
        move_east_(direction::east),
        move_west_(direction::west),
        move_northeast_(direction::northeast),
        move_northwest_(direction::northwest),
        move_southeast_(direction::southeast),
        move_southwest_(direction::southwest)
    {
        for (unsigned int i = 0; i < N_KEY_BINDINGS; ++i) {
            key_bindings_[i] = nullptr;
        }
    }

    void init_dvorak();   
};

#endif
