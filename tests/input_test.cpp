#include "io/curses.hpp"
#include <ncurses.h>

int main(int argc, char *argv[]) {
    curses::simple_start();
    int ch;
    
    printw("%d %d %d %d\n", KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN);
    printw("%d\n", KEY_ENTER);

    for(;;) {
        ch = getch();
        printw("%d\n", ch);
        switch (ch) {
            case KEY_LEFT: printw("left\n");break;
            case KEY_RIGHT: printw("right\n");break;
            case KEY_UP: printw("up\n");break;
            case KEY_DOWN: printw("down\n");break;
            case KEY_ENTER: printw("enter\n");break;
        }
    }

    curses::simple_stop();
    return 0;
}
