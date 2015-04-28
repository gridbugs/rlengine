#include "action/move_action.hpp"
#include "world/game_cell.hpp"

void move_action::on_complete(world &w) {
    game_cell& current = w.map.get_cell(character_->position);
    game_cell* destination = w.map.get_neighbour(current, direction_);
    if (destination != nullptr && !destination->is_solid()) {
        character_->position = destination->coord;
    }

    
}
