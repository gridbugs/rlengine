#ifndef _CHARACTER_IMAGE_HPP_
#define _CHARACTER_IMAGE_HPP_

#include "drawing/curses_col.hpp"

/* Information about a character available to an observer. */

class character_image {
    public:
    vec2<int> coord;
    char char_repr;
    pair_t col_pair_visible;
    pair_t col_pair_remembered;
    character_image(const vec2<int> &v, char c, pair_t vp, pair_t rp) :
        coord(v),
        char_repr(c),
        col_pair_visible(vp),
        col_pair_remembered(rp)
    {}
    
    char get_char() const {return char_repr;}
    pair_t get_col_pair_visible() const {return col_pair_visible;}
    pair_t get_col_pair_remembered() const {return col_pair_remembered;}
};

#endif
