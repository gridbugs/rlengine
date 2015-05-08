#include "behaviour/behaviour.hpp"
#include "world/fov.hpp"
#include "actor/character.hpp"

void behaviour::observe_world(const world &w) {
    for (std::vector<game_cell*>::iterator it = visible_cells_.begin(); 
        it != visible_cells_.end(); ++it) {
        game_cell *c = *it;
        
        knowledge_grid_.get_cell(c->coord).unsee();
    }

    visible_cells_.clear();
    fov_.push_visible_cells(character_.position, visible_cells_);

    for (std::vector<game_cell*>::iterator it = visible_cells_.begin(); 
        it != visible_cells_.end(); ++it) {
        game_cell *c = *it;

        knowledge_grid_.get_cell(c->coord).see();
    }
}
