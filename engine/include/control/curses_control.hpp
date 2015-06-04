#ifndef _CURSES_CONTRTOL_HPP_
#define _CURSES_CONTRTOL_HPP_

#include "io/curses.hpp"
#include "control/control.hpp"

template <typename C, typename W, typename T, typename K> class curses_control :
    public control<C, W, T, K> {

    protected:
    typedef control<C, W, T, K> control_t;
    typedef typename control_t::world_t world_t;
    typedef typename control_t::observer_t observer_t;
    typedef typename control_t::drawer_t drawer_t;
    typedef typename control_t::input_t input_t;

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
    curses_control(C &c, world_t &w, observer_t &o, drawer_t &d) :
        control_t(c, w, o, d)
    {
        for (unsigned int i = 0; i < N_KEY_BINDINGS; ++i) {
            key_bindings_[i] = control_t::INPUT_NONE;
        }
    }

    void init_dvorak() {
    
        key_bindings_[KEY_LEFT] = control_t::INPUT_WEST;
        key_bindings_[KEY_RIGHT] = control_t::INPUT_EAST;
        key_bindings_[KEY_UP] = control_t::INPUT_NORTH;
        key_bindings_[KEY_DOWN] = control_t::INPUT_SOUTH;
        
        key_bindings_['d'] = control_t::INPUT_WEST;
        key_bindings_['h'] = control_t::INPUT_SOUTH;
        key_bindings_['t'] = control_t::INPUT_NORTH;
        key_bindings_['n'] = control_t::INPUT_EAST;
        key_bindings_['f'] = control_t::INPUT_NORTHWEST;
        key_bindings_['g'] = control_t::INPUT_NORTHEAST;
        key_bindings_['x'] = control_t::INPUT_SOUTHWEST;
        key_bindings_['b'] = control_t::INPUT_SOUTHEAST;

    }
};

#endif
