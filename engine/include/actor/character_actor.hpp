#ifndef _CHARACTER_ACTOR_HPP_
#define _CHARACTER_ACTOR_HPP_

#include "character/character.hpp"
#include "world/world.hpp"
#include "fov/fov.hpp"
#include "actor/actor.hpp"
#include "actor/knowledge.hpp"
#include "drawing/actor_drawing_interface.hpp"

class character_actor : public actor, public actor_drawing_interface {
    protected:
    character &character_;
    virtual int act(world &w) = 0;
    virtual bool can_act() const = 0;
    
    fov &fov_;
    grid<knowledge_cell> knowledge_grid_;
    std::vector<game_cell_interface*> visible_cells_; // optimization for fast interface with fov_detector

    void observe_world(world &w);

    public:
    character_actor(character &c, world &w, fov &f) : 
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
