#include "actor/character_actor.hpp"

void character_actor::observe_world(world &w) {
    for (std::vector<game_cell_interface*>::iterator it = visible_cells_.begin(); 
        it != visible_cells_.end(); ++it) {
        game_cell_interface *c = *it;
        
        knowledge_grid_.get_cell(c->coord).unsee();
    }

    visible_cells_.clear();
    fov_.push_visible_cells(character_.position, visible_cells_);

    for (std::vector<game_cell_interface*>::iterator it = visible_cells_.begin(); 
        it != visible_cells_.end(); ++it) {
        game_cell_interface *c = *it;

        knowledge_grid_.get_cell(c->coord).see();
    }
}

const grid<knowledge_cell> &character_actor::get_knowledge_grid() const {
    return knowledge_grid_;
}

const character &character_actor::get_character() const {
    return character_;
}
