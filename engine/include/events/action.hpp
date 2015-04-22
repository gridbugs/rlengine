#ifndef _ACTION_HPP_
#define _ACTION_HPP_

#include "world/world.hpp"
#include "agents/character.hpp"
#include "geometry/directions.hpp"

class action {
    public:
    virtual void apply(world &w) const = 0;
    virtual ~action() {}
};

class empty_action : public action {
    void apply(world &w) const {}
};

class move_action : public action {
    protected:
    const character &character_;
    const directions::direction_t direction_;
    public:
    move_action(const character &c, const directions::direction_t d) :
        character_(c), direction_(d)
    {};

    void apply(world &w) const;
};

#endif
