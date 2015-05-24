#include "world/world.hpp"
#include "world/conway_generator.hpp"
#include "io/curses.hpp"
#include "drawing/curses_drawer.hpp"
#include "actor/player_actor.hpp"
#include <ncurses.h>

int main(int argc, char *argv[]) {
    curses::simple_start();
    srand(2);

    world w(100, 40);
    delete (new conway_generator())->generate(w);
    fov_detector fov(w.map);
    curses_drawer dr;

    character player(w.get_random_empty_cell().coord);
    player_actor actor(player, w, fov, dr);
    actor.init_dvorak();

    schedule s;

    s.register_callback(actor, 0);


    s.run_until_empty(w);
    
    curses::simple_stop();

    return 0;
}
