#include <iostream>

#include "world/world.hpp"
#include "world/generator.hpp"
#include "io/curses.hpp"
#include "control/ncurses_controller.hpp"
#include "drawing/curses_drawer.hpp"

int main(int argc, char *argv[]) {
    curses::simple_start();

    curses_drawer dr;
    ncurses_controller c;
    character ch(c, 20, 20);
    c.init_dvorak();
    world w(100, 40);
    border_generator g;
    g.generate(w);
    
    dr.draw_world(w);
    wmove(stdscr, ch.position.y, ch.position.x);
    waddch(stdscr, '@');
    for(;;) {
        action &a = ch.get_action(w);
        a.on_complete(w);
        dr.draw_world(w);
        wmove(stdscr, ch.position.y, ch.position.x);
        waddch(stdscr, '@');
    }
    
    curses::simple_stop();

    return 0;
}
