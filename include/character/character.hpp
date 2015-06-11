#ifndef _CHARACTER_HPP_
#define _CHARACTER_HPP_

#include "geometry/vec2.hpp"
#include "geometry/direction.hpp"
#include "drawing/curses_col.hpp"

class character {
    public:
    vec2<int> coord;
    char char_repr = '@';
    pair_t col_pair_visible = PAIR_WHITE;
    pair_t col_pair_remembered = PAIR_WHITE;
 
    int level_index = 0;

    character(const vec2<int> &v) :
        coord(v)
    {}

    character(const vec2<int> &v, char c, pair_t vp, pair_t rp) :
        coord(v),
        char_repr(c),
        col_pair_visible(vp),
        col_pair_remembered(rp)
    {}

    virtual void take_damage(int d) {}
    virtual int get_move_time() const {return 1;}
    virtual int get_melee_range() const {return 1;}

    virtual char get_char() const {return char_repr;}
    virtual pair_t get_col_pair_visible() const {return col_pair_visible;}
    virtual pair_t get_col_pair_remembered() const {return col_pair_remembered;}
};

#endif
