#include "control/curses_controller.hpp"
#include <ncurses.h>

unsigned int curses_controller::get_key() {
    return getch();
}

void curses_controller::init_arrows() {
    key_bindings_[KEY_LEFT] = &move_west_;
    key_bindings_[KEY_RIGHT] = &move_east_;
    key_bindings_[KEY_UP] = &move_north_;
    key_bindings_[KEY_DOWN] = &move_south_;
}
