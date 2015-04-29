#ifndef _BEHAVIOUR_HPP_
#define _BEHAVIOUR_HPP_

#include "actor/actor.hpp"

class character;

class behaviour : public actor {

    protected:
    character &character_;

    public:

    behaviour(character &c) : 
        character_(c)
    {}

};

#endif
