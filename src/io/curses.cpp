#include "io/curses.hpp"
#include <stdlib.h>

void curses::simple_start() {
    char envstr[] = "TERM=xterm-256color";
    putenv(envstr);

    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    start_color();
}

void curses::simple_stop() {
    endwin();
}
