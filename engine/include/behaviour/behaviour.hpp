#ifndef _BEHAVIOUR_HPP_
#define _BEHAVIOUR_HPP_

#include "actor/actor.hpp"
#include "actor/knowledge.hpp"

class character;

class behaviour : public actor {

    protected:
    character &character_;
    world &world_;
    grid<knowledge_cell> knowledge_grid_;

    public:

    behaviour(character &c, world &w) : 
        character_(c),
        world_(w),
        knowledge_grid_(w.width, w.height)
    {}

};

#endif
