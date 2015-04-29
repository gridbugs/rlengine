#ifndef _ACTION_SCHEDULE_HPP_
#define _ACTION_SCHEDULE_HPP_

#include <queue>
#include <vector>
#include <utility>

class action;
class world;

class action_schedule {

    public:
    typedef long long unsigned int absolute_time_t;

    protected:
    typedef std::pair<absolute_time_t, const action*> element_t;
    absolute_time_t time_;
    world &world_;
    
    class element_compare {
        public:
        bool operator()(const element_t &a, const element_t &b) {
            return a.first < b.first;
        }
    };

    std::priority_queue<
        element_t, 
        std::vector<element_t>,
        element_compare
    > pq_;


    public:
    void register_action(const action& a);
    const action* get_next_action();
    void step();

    action_schedule(world &w) : 
        time_(0),
        world_(w)
    {}
    absolute_time_t get_time() {return time_;}
};

#endif
