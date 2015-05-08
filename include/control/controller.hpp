#ifndef _CONTROLLER_HPP_
#define _CONTROLLER_HPP_

#include "action/action.hpp"
#include "actor/character.hpp"
#include "action/move_action.hpp"
#include "geometry/direction.hpp"
#include "behaviour/behaviour.hpp"


class controller : public behaviour {
    protected:

    move_action move_north_;
    move_action move_south_;
    move_action move_east_;
    move_action move_west_;
    move_action move_northeast_;
    move_action move_northwest_;
    move_action move_southeast_;
    move_action move_southwest_;

    public:
    controller(character &c, world &w, fov_detector &f) :
        behaviour(c, w, f),
        move_north_(c, direction::north),
        move_south_(c, direction::south),
        move_east_(c, direction::east),
        move_west_(c, direction::west),
        move_northeast_(c, direction::northeast),
        move_northwest_(c, direction::northwest),
        move_southeast_(c, direction::southeast),
        move_southwest_(c, direction::southwest)
    {}
    virtual bool can_act() {return true;}
};

#endif
