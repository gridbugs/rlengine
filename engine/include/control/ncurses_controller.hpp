#ifndef _NCURSES_CONTROLLER_HPP_
#define _NCURSES_CONTROLLER_HPP_

#include "control/key_controller.hpp"

class ncurses_controller : public key_controller {

    protected:
    unsigned int get_key();

    public:
    ncurses_controller() : key_controller() {}

};

#endif
