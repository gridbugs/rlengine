#include "control/key_controller.hpp"

key_controller::key_controller() {
    for (unsigned int i = 0; i < N_KEY_BINDINGS_; ++i) {
        key_bindings_[i] = nullptr;
    }
}

action& key_controller::get_action(world &w) {
    action *a = nullptr;
    while (a == nullptr) {
        unsigned int key_code = get_key();
        if (key_code >= N_KEY_BINDINGS_) {
            continue;
        }
        a = key_bindings_[key_code];
    }
    
    return *a;
}

void key_controller::init_dvorak() {
    key_bindings_['d'] = &move_west_;
    key_bindings_['h'] = &move_south_;
    key_bindings_['t'] = &move_north_;
    key_bindings_['n'] = &move_east_;
    key_bindings_['f'] = &move_northwest_;
    key_bindings_['g'] = &move_northeast_;
    key_bindings_['x'] = &move_southwest_;
    key_bindings_['b'] = &move_southeast_;
}
