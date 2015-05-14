#include "actor/player_character.hpp"

action& player_character::get_action(const world &w) {
    return test_action_;
}
