#ifndef _ALWAY_MOVE_LEFT_HPP_
#define _ALWAY_MOVE_LEFT_HPP_

#include "actor/character_actor.hpp"
#include "transaction/transaction.hpp"

class always_move_left : public character_actor {
    protected:
    void act(world &w) {
        w.register_transaction<try_move_transaction>(this->character_, direction::west);
    }
    bool can_act() const {return true;}

    public:
    always_move_left(character &c, observer &o) :
        character_actor(c, o)
    {}
};

#endif
