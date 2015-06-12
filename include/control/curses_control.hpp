#ifndef _CURSES_CONTRTOL_HPP_
#define _CURSES_CONTRTOL_HPP_

#include "io/curses.hpp"
#include "control/control.hpp"

class curses_control : public control {

    protected:
    typedef typename control::input_t input_t;

    static const unsigned int N_KEY_BINDINGS = 512;
    std::array<input_t, N_KEY_BINDINGS> key_bindings_;

    input_t get_input() const {
        unsigned int key_code;
        do {
            key_code = wgetch(curses::game_window);
        } while (key_code >= N_KEY_BINDINGS);
        return key_bindings_[key_code];
    }

    public:
    curses_control(character &c, observer &o, drawer &d) :
        control(c, o, d)
    {
        for (unsigned int i = 0; i < N_KEY_BINDINGS; ++i) {
            key_bindings_[i] = control::INPUT_NONE;
        }
    }

    void init_dvorak() {

        key_bindings_[KEY_LEFT] = control::INPUT_WEST;
        key_bindings_[KEY_RIGHT] = control::INPUT_EAST;
        key_bindings_[KEY_UP] = control::INPUT_NORTH;
        key_bindings_[KEY_DOWN] = control::INPUT_SOUTH;

        key_bindings_['d'] = control::INPUT_WEST;
        key_bindings_['h'] = control::INPUT_SOUTH;
        key_bindings_['t'] = control::INPUT_NORTH;
        key_bindings_['n'] = control::INPUT_EAST;
        key_bindings_['f'] = control::INPUT_NORTHWEST;
        key_bindings_['g'] = control::INPUT_NORTHEAST;
        key_bindings_['x'] = control::INPUT_SOUTHWEST;
        key_bindings_['b'] = control::INPUT_SOUTHEAST;

    }
};

#endif
