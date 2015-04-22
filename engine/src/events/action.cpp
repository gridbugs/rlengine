#include "events/action.hpp"
#include "agents/character.hpp"

void move_action::apply(world &w) const {
    game_cell &current = w.map.get_cell(character_.position);
    game_cell *next = w.map.get_neighbour(current, direction_);
    
    if (next == nullptr) {
        return;
    }

    character_.position.x = next->x_coord;
    character_.position.y = next->y_coord;
}
