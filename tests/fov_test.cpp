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
#include "world/fov.hpp"

#define RANDOM 0
#define DRAWING 1

int main(int argc, char *argv[]) {
#if DRAWING
    curses::simple_start();
#endif
#if RANDOM
    srand(time(NULL));
#else
    srand(0);
#endif
    
    world w(100, 40);
    fov_detector f(w.map);
    conway_generator g;
    g.generate(w);
    
//    character player(w.get_random_empty_cell().coord);
    character player(vec2<int>(22, 21));
//    std::cout << player.position << std::endl;
    curses_controller ctrl(player, w, f);
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
        ctrl.observe_world(w);
#if DRAWING
        dr.draw_world(w, ctrl);
        wmove(stdscr, player.position.y, player.position.x);
        waddch(stdscr, '@');
#endif        
        a->on_complete(w);
    }

#if DRAWING
    curses::simple_stop();
#endif
    return 0;
}
