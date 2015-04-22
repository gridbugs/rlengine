#ifndef _EVENT_HPP_
#define _EVENT_HPP_

#include <vector>
#include <utility>
#include "events/action.hpp"
#include "agents/character.hpp"
#include "geometry/directions.hpp"


class event {
    public:
    typedef long long int abs_time_t;

    protected:
    std::vector<std::pair<abs_time_t, action*>> sequence;
    unsigned int seq_idx;

    public:
    event() : seq_idx(0) {}

    abs_time_t get_next_time() const {return sequence[seq_idx].first;}

    bool is_complete() const {return seq_idx >= sequence.size();}

    const action& get_next_action() const {return *sequence[seq_idx].second;}

    virtual ~event() {
        for (std::vector<std::pair<abs_time_t, action*>>::iterator it = sequence.begin();
            it != sequence.end(); ++it) {
            
            delete it->second;
        }
    }
};

class move_event : public event {
    public:
    move_event(abs_time_t current_time,
               const character &c, const directions::direction_t d,
               const int warmup, const int cooldown) :
        event()
    {
        abs_time_t move_time = current_time + warmup;
        abs_time_t end_time = move_time + cooldown;

        sequence.push_back(
            std::pair<abs_time_t, action*>(move_time, new move_action(c, d)));
        sequence.push_back(
            std::pair<abs_time_t, action*>(end_time, new empty_action()));
    }

};

#endif
