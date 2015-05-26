#ifndef _SCHEDULE_HPP_
#define _SCHEDULE_HPP_

#include <queue>
#include <vector>

template <typename T> class schedule_callback;

template <typename T> class callback_registry {
    public:
    typedef long long unsigned int time_t;
    virtual void register_callback(schedule_callback<T>&, const time_t) = 0;
};

template <typename T> class schedule_callback {
    public:
    virtual void operator()(T& w, callback_registry<T>& cr) = 0;
};

template <typename T> class callback_loop {
    public:
    virtual void run_until_empty(T&) = 0;
};

template <typename T> class schedule : public callback_registry<T>, public callback_loop<T> {
    private:

    class event {
        public:
        schedule_callback<T> *fn;
        time_t time;
        event(schedule_callback<T> &fn, const time_t time) :
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

    void register_callback(schedule_callback<T> &fn, const typename callback_registry<T>::time_t relative_time) {
        pq_.push(event(fn, relative_time + absolute_time_));
    }

    void run_until_empty(T& w) {
        while (!pq_.empty()) {
            const event &e = pq_.top();
            absolute_time_ = e.time;
            schedule_callback<T> *c = e.fn;
            pq_.pop();
            (*c)(w, *this);
        }
    }

    time_t get_time() const {return absolute_time_;}
};

#endif
