#include "character/character.hpp"
#include "world/world.hpp"

void character::init_knowledge_grids() {
    for (int i = 0; i < world_.get_depth(); ++i) {
        knowledge_grids.push_back(knowledge_grid(world_.get_width(), world_.get_height()));
    }
}
