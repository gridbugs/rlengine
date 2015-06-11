#ifndef _ALWAY_MOVE_LEFT_HPP_
#define _ALWAY_MOVE_LEFT_HPP_

#include "actor/character_actor.hpp"
#include "transaction/transaction.hpp"

class always_move_left : public character_actor {
    protected:
    void act(world &w) {
        w.transactions.register_transaction(std::make_unique<try_move_transaction>(
            this->character_, direction::west));
    }
    bool can_act() const {return true;}

    public:
    always_move_left(character &c, world &w, observer &o) :
        character_actor(c, w, o)
    {}
};

#endif
