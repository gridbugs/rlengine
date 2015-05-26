#ifndef _HUD_HPP_
#define _HUD_HPP_

#include "schedule/schedule.hpp"
#include "world/world.hpp"

class hud {
    protected:
    const schedule<world> &schedule_;
    public:
    hud(const schedule<world> &s) :
        schedule_(s)
    {}
    
    typedef callback_registry<world>::time_t time_t;

    time_t get_time() const {return schedule_.get_time();}
};

#endif
