#include <iostream>
#include <ncurses.h>

#include "world/world.hpp"
#include "world/generator.hpp"
#include "world/conway_generator.hpp"
#include "io/curses.hpp"
#include "drawing/curses_drawer.hpp"
#include "actor/character.hpp"
#include "action/init_action.hpp"
#include "control/curses_controller.hpp"

int main(int argc, char *argv[]) {
    curses::simple_start();
    srand(time(NULL));
    
    world w(100, 40);
    conway_generator g;
    g.generate(w);
    
    character player(w.get_random_empty_cell().coord);
    curses_controller ctrl(player, w);
    player.set_behaviour(ctrl);
    ctrl.init_dvorak();
    ctrl.init_arrows();

    curses_drawer dr;

    w.schedule.register_action(init_action(player));
    
    for(;;) {
        
        const action* a = w.schedule.get_next_action();
        if (a == nullptr) {
            break;
        }
        a->apply(w);
        ctrl.observe_world();

        dr.draw_world(w, ctrl);
        wmove(stdscr, player.position.y, player.position.x);
        waddch(stdscr, '@');
        
        a->on_complete(w);
    }


    curses::simple_stop();
    return 0;
}
