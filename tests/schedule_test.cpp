#include "world/world.hpp"
#include "world/conway_generator.hpp"
#include "io/curses.hpp"
#include "drawing/curses_drawer.hpp"
#include "actor/player_actor.hpp"
#include "ui/hud.hpp"
#include <ncurses.h>

int main(int argc, char *argv[]) {
    curses::simple_start();
    srand(2);

    world w(100, 40);
    delete (new conway_generator())->generate(w);
    fov_detector fov(w.map);
    curses_drawer dr;
    schedule s;
    hud h(s);
    character player(w.get_random_empty_cell().coord);
    player_actor actor(player, w, fov, dr, h);
    actor.init_dvorak();


    s.register_callback(actor, 0);


    s.run_until_empty(w);
    
    curses::simple_stop();

    return 0;
}
