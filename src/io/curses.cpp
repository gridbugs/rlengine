#include "io/curses.hpp"
#include <stdlib.h>

WINDOW *curses::game_window = nullptr;
WINDOW *curses::console_window = nullptr;
WINDOW *curses::hud_window = nullptr;

static curses::streambuf buf;
std::ostream curses::cout(&buf);
std::ostream &(&curses::endl)(std::ostream &os) = std::endl;

void curses::simple_start() {
    char envstr[] = "TERM=xterm-256color";
    putenv(envstr);

    initscr();
    cbreak();
    noecho();
    curs_set(0);
    start_color();

    curses::game_window = newwin(
        GAME_WINDOW_HEIGHT,
        GAME_WINDOW_WIDTH,
        GAME_WINDOW_STARTY,
        GAME_WINDOW_STARTX
    );
    curses::console_window = newwin(
        CONSOLE_WINDOW_HEIGHT,
        CONSOLE_WINDOW_WIDTH,
        CONSOLE_WINDOW_STARTY,
        CONSOLE_WINDOW_STARTX
    );
    curses::hud_window = newwin(
        HUD_WINDOW_HEIGHT,
        HUD_WINDOW_WIDTH,
        HUD_WINDOW_STARTY,
        HUD_WINDOW_STARTX
    );

    keypad(curses::game_window, TRUE);

    scrollok(curses::console_window, true);
    buf.set_window(curses::console_window);
}

void curses::simple_stop() {
    endwin();
}
