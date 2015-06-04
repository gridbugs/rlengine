#ifndef _ALWAY_MOVE_LEFT_HPP_
#define _ALWAY_MOVE_LEFT_HPP_

#include "actor/character_actor.hpp"
#include "transaction/transaction.hpp"
#include <functional>

template <typename C, typename W, typename T, typename K> class always_move_left :
    public character_actor<C, W, T, K> {
    
    protected:
    typedef typename character_actor<C, W, T, K>::world_t world_t;

    void act(world_t &w) {
        w.transactions.register_transaction(std::make_unique<try_move_transaction<T, C, W>>(
            this->character_, direction::west));
    }
    bool can_act() const {return true;}

    public:
    always_move_left(C &c, world_t &w, observer<C, W, K> &o) :
        character_actor<C, W, T, K>(c, w, o)
    {}
};

#endif
