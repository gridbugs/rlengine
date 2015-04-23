#include "io/curses.hpp"

#include <ncurses.h>

int main(int argc, char *argv[]) {

    curses::simple_start();
    getch();
    curses::simple_stop();

    return 0;
}
