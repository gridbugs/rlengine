#ifndef _CHARACTER_ACTOR_HPP_
#define _CHARACTER_ACTOR_HPP_

#include "character/character.hpp"
#include "world/world.hpp"
#include "observer/observer.hpp"
#include "actor/actor.hpp"
#include "actor/knowledge.hpp"
#include "drawing/actor_drawing_interface.hpp"

template <typename C, typename W, typename K> class character_actor : 
    public actor<W>, public actor_drawing_interface {

    protected:
    character &character_;
    virtual int act(world<W> &w) = 0;
    virtual bool can_act() const = 0;
    
    observer<C, W, K> &observer_;
    grid<knowledge_cell> knowledge_grid_;

    void observe_world(world<W> &w) {
        observer_.observe(character_, w.map, knowledge_grid_);
    }

    public:
    character_actor(character &c, world<W> &w, observer<C,W,K> &f) : 
        character_(c),
        observer_(f),
        knowledge_grid_(w.width, w.height)
    {}

    void operator()(world<W>& w, callback_registry<world<W>>& cr) {
        if (can_act()) {
            observe_world(w);
            int cooldown = act(w);
            if (can_act()) {
                cr.register_callback(*this, cooldown);
            }
        }
    }
    
    const grid<knowledge_cell> &get_knowledge_grid() const {
        return knowledge_grid_;
    }
    const character &get_character() const {
        return character_;
    }
};


#endif
