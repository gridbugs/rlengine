#ifndef _ACTION_HPP_
#define _ACTION_HPP_

#include "behaviour/behaviour.hpp"
#include "geometry/direction.hpp"
#include "debug/fifo.hpp"


class action {
    public:
    virtual timed_result operator()() = 0;
    virtual bool is_possible() const = 0;
    virtual bool is_safe() const = 0;
};

template <typename C, typename W> class move_in_direction_action : public action {
    protected:
    typedef world<C, W> world_t;

    world_t &world_;
    C &character_;
    direction::direction_t &direction_;
    W *destination_;
    bool possible_;

    public:
    move_in_direction_action(world_t &w, C &c, direction::direction_t d) :
        world_(w),
        character_(c),
        direction_(d)
    {
        grid<W> &map = w.maps[c.level_index];
        destination_ = map.get_neighbour(map.get_cell(c.coord), d);
        possible_ = destination_ != nullptr && !destination_->is_solid();
    }

    bool is_possible() const {
        return possible_;
    }

    bool is_safe() const {
        return true;
    }

    timed_result operator()() {
        if (is_possible()) {
            character_.coord = destination_->coord;
            return timed_result(true, character_.get_move_time(direction_));
        }
        return timed_result(false, character_.get_move_time(direction_));
    }
};

template <typename C, typename W> class attack_in_direction_action : public action {
    protected:
    typedef world<C, W> world_t;
    public:
    attack_in_direction_action(world_t &w, C &c, direction::direction_t d)
    {}

    bool is_possible() const {
        return true;
    }

    bool is_safe() const {
        return true;
    }

    timed_result operator()() {
        fifo::cout << "attacking..." << fifo::endl;
        return timed_result(true, 1); //TODO
    }
};

#endif
