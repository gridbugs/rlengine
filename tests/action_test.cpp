#include <iostream>
#include "action/move_action.hpp"
#include "actor/player_character.hpp"
#include "geometry/direction.hpp"
#include "world/generator.hpp"

int main(int argc, char *argv[]) {
    
    world w(100, 100);
    border_generator g;
    g.generate(w);
    player_character pc(10, 10);
    move_action a(pc, direction::northeast);

    std::cout << pc.position << std::endl;
    a.on_complete(w);
    std::cout << pc.position << std::endl;

    return 0;
}
