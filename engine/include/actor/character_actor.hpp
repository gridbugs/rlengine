#ifndef _CHARACTER_ACTOR_HPP_
#define _CHARACTER_ACTOR_HPP_

#include "character/character.hpp"
#include "world/world.hpp"
#include "fov/fov.hpp"
#include "actor/actor.hpp"
#include "actor/knowledge.hpp"
#include "drawing/actor_drawing_interface.hpp"

template <typename W> class character_actor : public actor<W>, public actor_drawing_interface {
    protected:
    character &character_;
    virtual int act(world<W> &w) = 0;
    virtual bool can_act() const = 0;
    
    fov &fov_;
    grid<knowledge_cell> knowledge_grid_;
    std::vector<const vec2<int>*> visible_cells_; // optimization for fast interface with fov_detector

    void observe_world(world<W> &w) {
        for (std::vector<const vec2<int>*>::iterator it = visible_cells_.begin(); 
            it != visible_cells_.end(); ++it) {
            const vec2<int> *c = *it;
            
            knowledge_grid_.get_cell(*c).unsee();
        }

        visible_cells_.clear();
        fov_.push_visible_cells(character_.position, visible_cells_);

        for (std::vector<const vec2<int>*>::iterator it = visible_cells_.begin(); 
            it != visible_cells_.end(); ++it) {
            const vec2<int> *c = *it;

            knowledge_grid_.get_cell(*c).see();
        }
    }

    public:
    character_actor(character &c, world<W> &w, fov &f) : 
        character_(c),
        fov_(f),
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
