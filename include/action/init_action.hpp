#ifndef _INIT_ACTION_HPP_
#define _INIT_ACTION_HPP_

#include "action/action.hpp"

class init_action : public action {
    private:
    actor &actor_;
    public:
    init_action(actor &a) :
        actor_(a)
    {}

    void on_complete(world &w) const {
        w.schedule.register_action(actor_.get_action(w));
    }
};

#endif
