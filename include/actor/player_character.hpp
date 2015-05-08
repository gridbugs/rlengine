#ifndef _PLAYER_CHARACTER_HPP_
#define _PLAYER_CHARACTER_HPP_

#include "actor/character.hpp"

class player_character : public character {
    protected:
    action test_action_;
    public:
    action& get_action(world &w);
    bool can_act() {return true;}

    player_character(int x, int y) :
        character(x, y) {}
    player_character(vec2<int> v) :
        character(v) {}
};

#endif
