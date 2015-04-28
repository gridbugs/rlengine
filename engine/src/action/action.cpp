#include "action/action.hpp"
#include "world/world.hpp"
#include "actor/actor.hpp"

void sync_action::on_complete(world &w) {
    sync_on_complete(w);
    if (actor_->can_act()) {
        w.schedule.register_action(actor_->get_action(w));
    }
}

void chain_action::on_complete(world &w) {
    chain_on_complete(w);
    if (next_action_ != nullptr) {
        w.schedule.register_action(*next_action_);
    }
}
