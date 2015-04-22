#include "drawing/curses.hpp"
#include "drawing/curses_drawer.hpp"
#include "world/world.hpp"
#include "world/generator.hpp"


int main(int argc, char *argv[]) {
    curses::simple_start();
    world w(100, 40);
    generator &g = *new border_generator();
    drawer &d = *new curses_drawer();

    g.generate(w);

    d.draw_world(w);

    getch();
    

    delete &g;
    curses::simple_stop();
    return 0;
}
