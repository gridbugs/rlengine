#ifndef _CHARACTER_ACTOR_HPP_
#define _CHARACTER_ACTOR_HPP_

#include "world/world.hpp"
#include "observer/observer.hpp"
#include "actor/actor.hpp"
#include "drawing/actor_drawing_interface.hpp"

template <typename C, typename W, typename K> class character_actor : 
    public actor<W>, public actor_drawing_interface<K> {

    protected:
    C &character_;
    virtual int act(world<W> &w) = 0;
    virtual bool can_act() const = 0;
    
    observer<C, W, K> &observer_;
    std::vector<grid<K>> knowledge_grids_;

    grid<W> &get_current_grid(world<W> &w) const {
        return w.maps[character_.level_index];
    }

    grid<K> &get_current_knowledge_grid() {
        return knowledge_grids_[character_.level_index];
    }
    
    const grid<K> &get_current_knowledge_grid() const {
        return knowledge_grids_[character_.level_index];
    }

    void observe_world(world<W> &w) {
        observer_.observe(character_, get_current_grid(w), 
                            this->get_current_knowledge_grid());
    }

    public:
    character_actor(C &c, world<W> &w, observer<C,W,K> &f) : 
        character_(c),
        observer_(f)
    {
        for (typename std::vector<grid<W>>::iterator it = w.maps.begin();
             it != w.maps.end(); ++it) {
            knowledge_grids_.push_back(grid<K>(it->width, it->height));
        }
    }

    void operator()(world<W>& w, callback_registry<world<W>>& cr) {
        if (can_act()) {
            observe_world(w);
            int cooldown = act(w);
            if (can_act()) {
                cr.register_callback(*this, cooldown);
            }
        }
    }
    
    const grid<K> &get_knowledge_grid() const {
        return this->get_current_knowledge_grid();
    }
    const C &get_character() const {
        return character_;
    }
};


#endif
