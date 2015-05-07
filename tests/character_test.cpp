#include <iostream>

#include "world/world.hpp"
#include "world/generator.hpp"
#include "io/curses.hpp"
#include "control/curses_controller.hpp"
#include "drawing/curses_drawer.hpp"
#include "action/init_action.hpp"

int main(int argc, char *argv[]) {
    curses::simple_start();

    curses_drawer dr;
    world w(100, 40);
    character ch(20, 20);
    curses_controller c(ch, w);
    ch.set_behaviour(c);
    c.init_dvorak();
    c.init_arrows();
    border_generator g;
    g.generate(w);

    w.schedule.register_action(init_action(ch));
    
    for(;;) {
        
        const action* a = w.schedule.get_next_action();
        if (a == nullptr) {
            break;
        }
        a->apply(w);

        dr.draw_world(w);
        wmove(stdscr, ch.position.y, ch.position.x);
        waddch(stdscr, '@');
        
        a->on_complete(w);
    }
    
    curses::simple_stop();

    return 0;
}
