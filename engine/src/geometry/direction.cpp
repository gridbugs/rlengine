#include "geometry/direction.hpp"

#include <iostream>
#include <string>


using namespace direction;

static const direction_t ordinal_table[n_directions][n_directions] = {
    [north] = {
        [north] = none,
        [east] = northeast,
        [south] = none,
        [west] = northwest
    },
    [east] = {
        [north] = northeast,
        [east] = none,
        [south] = southeast,
        [west] = none
    },
    [south] = {
        [north] = none,
        [east] = southeast,
        [south] = none,
        [west] = southwest
    },
    [west] = {
        [north] = northwest,
        [east] = none,
        [south] = southwest,
        [west] = none
    }
};

static const direction_t cardinal_table[n_directions - n_cardinal_directions][2] = {
    [northeast-n_cardinal_directions] = {north, east},
    [southeast-n_cardinal_directions] = {south, east},
    [southwest-n_cardinal_directions] = {south, west},
    [northwest-n_cardinal_directions] = {north, west}
};


bool direction::is_cardinal(direction_t d) {
    return d < n_cardinal_directions;
}
bool direction::is_ordinal(direction_t d) {
    return d >= n_cardinal_directions && d != none;
}

direction_t direction::combine_cardinal_directions(direction_t a, direction_t b) {
    if (a == none || b == none) {
        return none;
    }
    if (is_ordinal(a) || is_ordinal(b)) {
        return none;
    }
    return ordinal_table[a][b];
}

void direction::split_ordinal_direction(const direction_t ordinal,
                                        direction_t *c0,
                                        direction_t *c1) {
    if (is_ordinal(ordinal)) {   
        int idx = ordinal - n_cardinal_directions;
        *c0 = cardinal_table[idx][0];
        *c1 = cardinal_table[idx][1];
    } else {
        *c0 = none;
        *c1 = none;
    }
}

static const std::string string_table[n_directions + 1] = {
    [north] = "north",
    [east] = "east",
    [south] = "south",
    [west] = "west",
    [northeast] = "northeast",
    [southeast] = "southeast",
    [southwest] = "southwest",
    [northwest] = "northwest",
    [none] = "none" 
};

std::ostream &operator<<(std::ostream &out, const direction_t dir) {
    return out << string_table[dir];
}
