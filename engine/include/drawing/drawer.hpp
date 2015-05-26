#ifndef _DRAWER_HPP
#define _DRAWER_HPP

#include "world/world.hpp"
#include "character/character.hpp"
#include "drawing/actor_drawing_interface.hpp"

template <typename W, typename K> class drawer {
    public:
    virtual void draw_world_from_actor(world<W> &w, const actor_drawing_interface<K> &a) = 0;
};

#endif
