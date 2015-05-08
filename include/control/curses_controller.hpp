#ifndef _CURSES_CONTROLLER_HPP_
#define _CURSES_CONTROLLER_HPP_

#include "control/key_controller.hpp"

class curses_controller : public key_controller {

    protected:
    unsigned int get_key();

    public:
    curses_controller(character &c, world &w) : 
        key_controller(c, w) 
    {
        for (unsigned int i = 0; i < N_KEY_BINDINGS_; ++i) {
            key_bindings_[i] = nullptr;
        }
    }

    void init_arrows();
};

#endif
