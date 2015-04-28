#ifndef _KEY_CONTROLLER_HPP_
#define _KEY_CONTROLLER_HPP_

#include "control/controller.hpp"
#include <array>

class key_controller : public controller {
 
    protected:
    static const unsigned int N_KEY_BINDINGS_ = 512;
    std::array<action*, N_KEY_BINDINGS_> key_bindings_;

    virtual unsigned int get_key() = 0;   

    public:
    key_controller();
    action& get_action(world &w);
    void init_dvorak();
};

#endif
