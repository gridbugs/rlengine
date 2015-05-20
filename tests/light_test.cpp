
#include <iostream>
#include <ncurses.h>
#include <assert.h>

#include "world/world.hpp"
#include "world/generator.hpp"
#include "world/conway_generator.hpp"
#include "io/curses.hpp"
#include "drawing/curses_drawer.hpp"
#include "actor/character.hpp"
#include "action/init_action.hpp"
#include "control/curses_controller.hpp"
#include "world/fov.hpp"
#include "debug/fifo.hpp"

#define RANDOM 0
#define DRAWING 1

int main(int argc, char *argv[]) {
    fifo::start();
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
    
    character player(vec2<int>(14, 26));
    curses_controller ctrl(player, w, f);
    player.set_behaviour(ctrl);
    ctrl.init_dvorak();
    ctrl.init_arrows();

    curses_drawer dr;
    //dr.test();
    //for(;;);
#if 1
    w.schedule.register_action(init_action(player));
    int i = 0;
        
    for(;;) {
        
        const action* a = w.schedule.get_next_action();
        if (a == nullptr) {
            break;
        }
        a->apply(w);
        ctrl.observe_world(w);
#if DRAWING
        dr.draw_world(w, ctrl, player);
        /*
        wmove(curses::game_window, player.position.y, player.position.x);
        waddch(curses::game_window, '@');
        wrefresh(curses::game_window);
        */
        curses::cout << "test " << i << curses::endl;
        fifo::cout << "test " << i++ << fifo::endl;
        wrefresh(curses::console_window);
#endif        
        a->on_complete(w);
    }
#endif
#if DRAWING
    curses::simple_stop();
#endif
    fifo::stop();
    return 0;
}
