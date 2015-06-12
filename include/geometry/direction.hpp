#ifndef _DIRECTIONS_HPP_
#define _DIRECTIONS_HPP_

#include <iostream>

namespace direction {

    typedef enum {
        north = 0,
        east,
        south,
        west,
        northeast,
        southeast,
        southwest,
        northwest,
        none
    } direction_t;

    const int n_directions = 8;
    const int n_cardinal_directions = 4;
    const int n_ordinal_directions = 4;

    bool is_cardinal(direction_t d);
    bool is_ordinal(direction_t d);

    direction_t combine_cardinal_directions(direction_t a, direction_t b);
    void split_ordinal_direction(const direction_t ordinal,
                                 direction_t *c0,
                                 direction_t *c1);


    namespace cardinal {
        typedef enum {
            north = 0,
            east,
            south,
            west
        } direction_t;
    };

    namespace ordinal {
        typedef enum {
            northeast = 0,
            southeast,
            southwest,
            northwest
        } direction_t;
    };

};

std::ostream &operator<<(std::ostream &out, const direction::direction_t dir);

#endif
