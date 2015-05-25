#ifndef _SCHEDULE_HPP_
#define _SCHEDULE_HPP_

#include <queue>
#include <vector>
#include "world/world.hpp"

class schedule_callback;

class callback_registry {
    public:
    typedef long long unsigned int time_t;
    virtual void register_callback(schedule_callback&, const time_t) = 0;
};

class schedule_callback {
    public:
    virtual void operator()(world& w, callback_registry& cr) = 0;
};

class callback_loop {
    public:
    virtual void run_until_empty(world&) = 0;
};

class schedule : public callback_registry, public callback_loop {
    private:

    class event {
        public:
        schedule_callback *fn;
        time_t time;
        event(schedule_callback &fn, const time_t time) :
            fn(&fn),
            time(time)
        {}
    };

    class event_comparitor {
        public:
        bool operator()(const event &a, const event &b) {
            return a.time < b.time;
        }
    };

    std::priority_queue<event, std::vector<event>, event_comparitor> pq_;
    time_t absolute_time_;

    public:

    schedule() : absolute_time_(0) {}

    void register_callback(schedule_callback &fn, const time_t relative_time) {
        pq_.push(event(fn, relative_time + absolute_time_));
    }

    void run_until_empty(world& w) {
        while (!pq_.empty()) {
            const event &e = pq_.top();
            absolute_time_ = e.time;
            schedule_callback *c = e.fn;
            pq_.pop();
            (*c)(w, *this);
        }
    }

    time_t get_time() const {return absolute_time_;}
};

#endif
