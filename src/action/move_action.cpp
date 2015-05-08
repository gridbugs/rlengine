#include "action/move_action.hpp"
#include "world/game_cell.hpp"

void move_action::apply(world &w) const {
    game_cell& current = w.map.get_cell(character_->position);
    game_cell* destination = w.map.get_neighbour(current, direction_);
    if (destination != nullptr && !destination->is_solid()) {
        character_->position = destination->coord;
    }
}

void move_action::on_complete(world &w) const {
    w.schedule.register_action(character_->get_action(w));
}

int move_action::get_duration() const {
    return 1000 / character_->get_agility();
}
