#ifndef _HUD_HPP_
#define _HUD_HPP_

#include "schedule/schedule.hpp"

class hud {
    protected:
    const schedule &schedule_;
    public:
    hud(const schedule &s) :
        schedule_(s)
    {}
    
    typedef callback_registry::time_t time_t;

    time_t get_time() const {return schedule_.get_time();}
};

#endif
