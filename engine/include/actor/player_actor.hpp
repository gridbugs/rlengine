#ifndef _PLAYER_ACTOR_HPP_
#define _PLAYER_ACTOR_HPP_

#include <array>
#include <functional>
#include "actor/character_actor.hpp"
#include "geometry/direction.hpp"
#include "drawing/drawer.hpp"

template <typename C, typename W, typename K> class player_actor : 
    public character_actor<C, W, K> {

    protected:
    int act(world<C, W> &w) {
        drawer_.draw_world_from_actor(w, *this);

        action *a = nullptr;
        while (a == nullptr) {
            unsigned int key_code = wgetch(curses::game_window);  
            if (key_code >= N_KEY_BINDINGS) {
                continue;
            }
            a = key_bindings_[key_code];
        }
        
        return (*a)(this->character_, w);
    }

    bool can_act() const {return true;}
    drawer<C, W, K> &drawer_; // XXX separate this into a different class

    class action {
        public:
        virtual int operator()(C &c, world<C, W> &w) = 0;
    };

    class move_action : public action {
        private:
        direction::direction_t direction_;
        public:
        move_action(direction::direction_t d) : direction_(d) {}
        int operator()(C &c, world<C, W> &w) {
            grid<W> &map = w.maps[c.level_index];

            W *cell_ptr = map.get_neighbour(map.get_cell(c.coord), direction_);
            if (cell_ptr != nullptr) {
                w.move_character(c, cell_ptr->coord);
            }

            return 4; // TODO base this off character's stats
        }
    };

    static const unsigned int N_KEY_BINDINGS = 512;
    std::array<action*, N_KEY_BINDINGS> key_bindings_;

    move_action move_north_;
    move_action move_south_;
    move_action move_east_;
    move_action move_west_;
    move_action move_northeast_;
    move_action move_northwest_;
    move_action move_southeast_;
    move_action move_southwest_;

    public:
    
    player_actor(C &c, world<C, W> &w, observer<C, W, K> &o, drawer<C, W, K> &d) :
        character_actor<C, W, K>(c, w, o),
        drawer_(d),
        move_north_(direction::north),
        move_south_(direction::south),
        move_east_(direction::east),
        move_west_(direction::west),
        move_northeast_(direction::northeast),
        move_northwest_(direction::northwest),
        move_southeast_(direction::southeast),
        move_southwest_(direction::southwest)
    {
        for (unsigned int i = 0; i < N_KEY_BINDINGS; ++i) {
            key_bindings_[i] = nullptr;
        }
    }

    void init_dvorak() {
    
        key_bindings_[KEY_LEFT] = &move_west_;
        key_bindings_[KEY_RIGHT] = &move_east_;
        key_bindings_[KEY_UP] = &move_north_;
        key_bindings_[KEY_DOWN] = &move_south_;
        
        key_bindings_['d'] = &move_west_;
        key_bindings_['h'] = &move_south_;
        key_bindings_['t'] = &move_north_;
        key_bindings_['n'] = &move_east_;
        key_bindings_['f'] = &move_northwest_;
        key_bindings_['g'] = &move_northeast_;
        key_bindings_['x'] = &move_southwest_;
        key_bindings_['b'] = &move_southeast_;

    }
};

#endif
