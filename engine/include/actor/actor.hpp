#ifndef _ACTOR_HPP_
#define _ACTOR_HPP_

#include "character/character.hpp"
#include "schedule/schedule.hpp"
#include "world/world.hpp"
#include "world/fov.hpp"
#include "actor/knowledge.hpp"
#include "drawing/actor_drawing_interface.hpp"

class actor : public schedule_callback {};

class character_actor : public actor, public actor_drawing_interface {
    protected:
    character &character_;
    virtual int act(world &w) = 0;
    virtual bool can_act() const = 0;
    
    fov_detector &fov_;
    grid<knowledge_cell> knowledge_grid_;
    std::vector<game_cell*> visible_cells_; // optimization for fast interface with fov_detector

    void observe_world(world &w);

    public:
    character_actor(character &c, world &w, fov_detector &f) : 
        character_(c),
        fov_(f),
        knowledge_grid_(w.width, w.height)
    {}

    void operator()(world& w, callback_registry& cr) {
        if (can_act()) {
            observe_world(w);
            int cooldown = act(w);
            if (can_act()) {
                cr.register_callback(*this, cooldown);
            }
        }
    }
    
    const grid<knowledge_cell> &get_knowledge_grid() const;
    const character &get_character() const;
};


#endif
