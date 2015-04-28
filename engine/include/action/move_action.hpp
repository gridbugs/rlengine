#ifndef _MOVE_ACTION_HPP_
#define _MOVE_ACTION_HPP_

#include "action/action.hpp"
#include "actor/character.hpp"
#include "geometry/direction.hpp"

class move_action : public action {

    protected:
    character *character_;
    direction::direction_t direction_;

    public:
    move_action(){}
    move_action(character &c, direction::direction_t d) :
        character_(&c),
        direction_(d)
    {}

    void on_complete(world &w);
};

#endif
