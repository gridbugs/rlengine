#ifndef _DRAWER_HPP
#define _DRAWER_HPP

#include "world/world.hpp"
#include "character/character.hpp"
#include "drawing/actor_drawing_interface.hpp"

class drawer {
    public:
    virtual void draw_world_from_actor(world &w, const actor_drawing_interface &a) = 0;
};

#endif
