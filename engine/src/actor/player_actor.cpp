#include "actor/player_actor.hpp"
#include "io/curses.hpp"

void player_actor::init_dvorak() {
    
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

int player_actor::act(world<game_cell> &w) {
 
    drawer_.draw_world_from_actor(w, *this);
    drawer_.draw_hud(w, hud_);

    action *a = nullptr;
    while (a == nullptr) {
        unsigned int key_code = wgetch(curses::game_window);  
        if (key_code >= N_KEY_BINDINGS) {
            continue;
        }
        a = key_bindings_[key_code];
    }
    
    return (*a)(character_, w);
}

int player_actor::move_action::operator()(character &c, world<game_cell> &w) {
    game_cell *cell_ptr = w.map.get_neighbour(w.map.get_cell(c.position), direction_);
    if (cell_ptr != nullptr) {
        w.move_character(c, cell_ptr->coord);
    }

    return 1; // TODO base this off character's stats
}
