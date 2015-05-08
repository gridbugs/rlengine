#include "behaviour/behaviour.hpp"
#include "world/fov.hpp"
#include "actor/character.hpp"

void behaviour::observe_world() {
    fov::compute_fov(character_.position, world_.map, knowledge_grid_);
}
