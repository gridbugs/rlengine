#ifndef _SIGHTED_ACTOR_HPP_
#define _SIGHTED_ACTOR_HPP_

#include "actor/actor.hpp"
#include "world/world.hpp"
#include "world/fov.hpp"
#include "actor/knowledge.hpp"
#include "drawing/actor_drawing_interface.hpp"

class sighted_actor : public character_actor, public actor_drawing_interface  {

    protected:
    fov_detector &fov_;
    grid<knowledge_cell> knowledge_grid_;
    std::vector<game_cell*> visible_cells_; // optimization for fast interface with fov_detector

    void observe_world(world &w);
    int act(world &w);
    virtual int act_after_seeing(world &w) = 0;

    public:
    sighted_actor(character &c, world &w, fov_detector &f) : 
        character_actor(c), 
        fov_(f),
        knowledge_grid_(w.width, w.height)
    {}
 
    const grid<knowledge_cell> &get_knowledge_grid() const;
    const character &get_character() const;
};

#endif
