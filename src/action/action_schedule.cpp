#include "action/action_schedule.hpp"
#include "action/action.hpp"

void action_schedule::register_action(const action &a) {
    pq_.push(element_t(get_time() + a.get_duration(), &a));
    a.on_start(world_);
}

const action* action_schedule::get_next_action() {
    while (!pq_.empty()) {
        const action* a = pq_.top().second;
        pq_.pop();
        if (a->is_active()) {
            return a;
        }
    }
    return nullptr;
}

void action_schedule::step() {
    const action* a = get_next_action();
    if (a != nullptr) {
        a->apply(world_);
        a->on_complete(world_);
    }
}
