#ifndef _CHARACTER_HPP_
#define _CHARACTER_HPP_

#include <geometry/vec2.hpp>

class character {

    public:
    vec2<unsigned int> position;
    character(const unsigned int x, const unsigned int y) :
        position(x, y)
    {}

    virtual bool is_player_character() {return false;}
};

class player_character : public character {
    public:
    player_character(const unsigned int x, const unsigned int y) :
        character(x, y)
    {}

    bool is_player_character() {return true;}
};

#endif
