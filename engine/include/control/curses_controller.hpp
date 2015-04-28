#ifndef _CURSES_CONTROLLER_HPP_
#define _CURSES_CONTROLLER_HPP_

#include "control/key_controller.hpp"

class curses_controller : public key_controller {

    protected:
    unsigned int get_key();

    public:
    curses_controller() : key_controller() {}
    void init_arrows();
};

#endif
