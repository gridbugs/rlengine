#include "agents/character.hpp"
#include "events/action.hpp"
#include "geometry/directions.hpp"
#include <iostream>


int main(int argc, char *argv[]) {

    world w(100, 40);
    character *pc = new player_character(4, 7);
    action *a = new move_action(*pc, directions::southwest);

    std::cout << pc->position << std::endl;

    a->apply(w);
    
    std::cout << pc->position << std::endl;

    return 0;
}
