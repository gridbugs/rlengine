#ifndef _CHARACTER_ACTOR_HPP_
#define _CHARACTER_ACTOR_HPP_

#include "world/world.hpp"
#include "observer/observer.hpp"
#include "actor/actor.hpp"
#include "drawing/actor_drawing_interface.hpp"

template <typename C, typename W, typename T, typename K> class character_actor : 
    public actor<C, W, T>, public actor_drawing_interface<C, K> {

    protected:

    typedef world<C, W, T> world_t;
    typedef observer<C, W, K> observer_t;

    C &character_;
    virtual int act(world_t &w) = 0;
    virtual bool can_act() const = 0;
    
    observer_t &observer_;
    std::vector<grid<K>> knowledge_grids_;

    const grid<W> &get_current_grid(world_t &w) const {
        return w.maps[character_.level_index];
    }
    
    grid<W> &get_current_grid(world_t &w) {
        return w.maps[character_.level_index];
    }

    grid<K> &get_current_knowledge_grid() {
        return knowledge_grids_[character_.level_index];
    }
    
    const grid<K> &get_current_knowledge_grid() const {
        return knowledge_grids_[character_.level_index];
    }

    void observe_world(world_t &w) {
        observer_.observe(character_, get_current_grid(w), 
                            this->get_current_knowledge_grid());

        get_current_knowledge_grid().for_each([](K &k) {
            if (k.is_visible()) {
                k.unsee_characters();
            }
        });

        for (typename std::vector<std::unique_ptr<C>>::iterator it = w.characters.begin(); it != w.characters.end(); ++it) {
            if ((*it)->level_index == character_.level_index) {
                K &k = get_current_knowledge_grid().get_cell((*it)->coord);
                if (k.is_visible()) {
                    k.see_character(**it);   
                }
            }
        }
    }

    public:
    character_actor(C &c, world_t &w, observer_t &o) : 
        character_(c),
        observer_(o)
    {
        for (typename std::vector<grid<W>>::iterator it = w.maps.begin();
             it != w.maps.end(); ++it) {
            knowledge_grids_.push_back(grid<K>(it->width, it->height));
        }
    }

    void operator()(world_t& w, callback_registry<world_t>& cr) {
        if (can_act()) {
            observe_world(w);
            act(w);
            int cooldown = w.transactions.process_all(w);
//            cooldown = 1;
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
