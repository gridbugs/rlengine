#include "control/ncurses_controller.hpp"
#include <ncurses.h>

unsigned int ncurses_controller::get_key() {
    return getch();
}
