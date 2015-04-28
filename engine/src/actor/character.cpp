#include "actor/character.hpp"

bool character::set_behaviour(behaviour &b) {
    if (behaviour_ != nullptr) {
        if (behaviour_->deregister_character(*this)) {
            behaviour_ = nullptr;
        } else {
            return false;
        }
    }
    if (b.register_character(*this)) {
        behaviour_ = &b;
        return true;
    }
    return false;
}
