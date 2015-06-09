#ifndef _TRANSACTION_HPP_
#define _TRANSACTION_HPP_

#include "geometry/direction.hpp"
#include "world/world.hpp"
#include "io/curses.hpp"
#include <memory>

template <typename T, typename C, typename W> class move_transaction : public T {
    public:
    C &character_;
    const W &from_cell_;
    const W &to_cell_;
    move_transaction(C &c, const W &f, const W &t) :
        character_(c),
        from_cell_(f),
        to_cell_(t)
    {}

    int operator()(world<C, W, T> &w) {
        character_.coord = to_cell_.coord;
        return character_.get_move_time();
    }
};

template <typename T, typename C, typename W> class move_blocked_transaction : public T {
    public:
    C &character_;
    move_blocked_transaction(C &c) :
        character_(c)
    {}
    int operator()(world<C, W, T> &w) {
        return 1;
    }
};


template <typename T, typename C, typename W> class try_move_transaction : public T {
    public:
    C &character_;
    direction::direction_t direction_;
    try_move_transaction(C& c, direction::direction_t d) :
        character_(c),
        direction_(d)
    {}

    int operator()(world<C, W, T> &w) {
        grid<W> &map = w.maps[character_.level_index];
        bool success = false;
        map.with_neighbour(map.get_cell(character_.coord), direction_, [&](const W &dest) {
            if (!dest.is_solid()) {
                w.transactions.register_transaction(
                    std::make_unique<move_transaction<T, C, W>>(
                        character_,
                        map.get_cell(character_.coord),
                        dest
                    )
                );
                success = true;
            }
        });

        if (!success) {
            w.transactions.register_transaction(
                std::make_unique<move_blocked_transaction<T, C, W>>(
                    character_
                )
            );
        }
        return 0;
    }
};

template <typename T, typename C, typename W> class attack_transaction : public T {
    public:
    C &attacker_;
    C &target_;
    attack_transaction(C &a, C &t) :
        attacker_(a),
        target_(t)
    {}

    int operator()(world<C, W, T> &w) {
        curses::cout << "attacking!" << curses::endl;
        return 1;
    }
};

template <typename T, typename C, typename W> class miss_attack_transaction : public T {
    public:
    C &attacker_;
    miss_attack_transaction(C &a) :
        attacker_(a)
    {}

    int operator()(world<C, W, T> &w) {
        return 1;
    }
};

template <typename T, typename C, typename W> class try_attack_transaction : public T {
    public:
    C &attacker_;
    direction::direction_t direction_;
    try_attack_transaction(C& c, direction::direction_t d) :
        attacker_(c),
        direction_(d)
    {}

    int operator()(world<C, W, T> &w) {
        grid<W> &map = w.maps[attacker_.level_index];
        bool success = false;
        map.with_neighbour(map.get_cell(attacker_.coord), direction_, [&](const W &target_cell) {
            w.with_character_at_coord(attacker_.level_index, target_cell.coord, [&](C &target) {
                w.transactions.register_transaction(
                    std::make_unique<attack_transaction<T, C, W>>(
                        attacker_,
                        target
                    )
                );
                success = true;
            });
        });
        if (!success) {
             w.transactions.register_transaction(
                std::make_unique<miss_attack_transaction<T, C, W>>(attacker_)
            );
        }
        return 0;
    }
};

template <typename T, typename C, typename W, typename K> class see_character_transaction : public T {
    public:
    C &seer_;
    C &seen_;
    K &kcell_;
    see_character_transaction(C &seer, C &seen, K &kcell) :
        seer_(seer),
        seen_(seen),
        kcell_(kcell)
    {}
    int operator()(world<C, W, T> &w) {
        kcell_.see_character(seen_);
        return 0;
    }
};

template <typename T, typename C, typename W> class transaction {
    public:
    virtual int operator()(world<C, W, T> &w) = 0;
};

#endif
