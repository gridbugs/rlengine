#ifndef _TRANSACTION_HPP_
#define _TRANSACTION_HPP_

#include "geometry/direction.hpp"
#include "io/curses.hpp"
#include "world/world_cell.hpp"
#include "world/grid.hpp"
#include "knowledge/knowledge.hpp"
#include "character/character.hpp"

#include <memory>

class world;

class transaction {
    public:
    virtual int operator()(world &w) = 0;
};

class instant_transaction : public transaction {
    public:

    virtual void commit(world &w) = 0;

    int operator()(world &w) {
        commit(w);
        return 0;
    }
};


class try_transaction : public instant_transaction {
    public:
    virtual bool attempt(world &w) = 0;
    virtual void fail(world &w) = 0;

    void commit(world &w) {
        if (!attempt(w)) {
            fail(w);
        }
    }
};

class move_transaction : public transaction {
    public:
    character &character_;
    const world_cell &from_cell_;
    const world_cell &to_cell_;
    move_transaction(character &c, const world_cell &f, const world_cell &t) :
        character_(c),
        from_cell_(f),
        to_cell_(t)
    {}

    int operator()(world &w) {
        character_.coord = to_cell_.coord;
        return character_.get_move_time();
    }
};

class move_blocked_transaction : public transaction {
    public:
    character &character_;
    move_blocked_transaction(character &c) :
        character_(c)
    {}
    int operator()(world &w) {
        return 1;
    }
};


class try_move_transaction : public try_transaction {
    public:
    character &character_;
    direction::direction_t direction_;
    try_move_transaction(character& c, direction::direction_t d) :
        character_(c),
        direction_(d)
    {}

    bool attempt(world &w);
    void fail(world &w);
};

class attack_transaction : public transaction {
    public:
    character &attacker_;
    character &target_;
    attack_transaction(character &a, character &t) :
        attacker_(a),
        target_(t)
    {}

    int operator()(world &w) {
        curses::cout << "attacking!" << curses::endl;
        return 1;
    }
};

class miss_attack_transaction : public transaction {
    public:
    character &attacker_;
    miss_attack_transaction(character &a) :
        attacker_(a)
    {}

    int operator()(world &w) {
        return 1;
    }
};

class try_attack_transaction : public try_transaction {
    public:
    character &attacker_;
    direction::direction_t direction_;
    try_attack_transaction(character& c, direction::direction_t d) :
        attacker_(c),
        direction_(d)
    {}

    bool attempt(world &w);
    void fail(world &w);
};

class see_character_transaction : public instant_transaction {

    public:
    character &seer_;
    character &seen_;
    knowledge_cell &kcell_;
    see_character_transaction(character &seer, character &seen, knowledge_cell &kcell) :
        seer_(seer),
        seen_(seen),
        kcell_(kcell)
    {}
    void commit(world &w) {
        kcell_.see_character(seen_);
    }
};


#endif
