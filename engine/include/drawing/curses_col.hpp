#ifndef _CURSES_COL_HPP_
#define _CURSES_COL_HPP_

#include "io/curses.hpp"

typedef enum {
    COL_LIGHT_GREY = 16,
    COL_GREY,
    COL_BLACK,
    COL_RED,
    COL_DARK_RED,
    COL_WHITE
} col_t;

typedef enum {
    PAIR_VISIBLE = 16,
    PAIR_REMEMBERED,
    PAIR_UNKNOWN,
    PAIR_BLUE,
    PAIR_RED,
    PAIR_DARK_RED,
    PAIR_GREEN,
    PAIR_CYAN,
    PAIR_WHITE
} pair_t;

#endif
