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

    virtual bool register_character(character &c);

    bool can_act() {return true;}
};

#endif
