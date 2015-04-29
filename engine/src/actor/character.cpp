#include "actor/character.hpp"

void character::set_behaviour(behaviour &b) {
    behaviour_ = &b;
}
