#ifndef _ACTOR_DRAWING_INTERFACE_
#define _ACTOR_DRAWING_INTERFACE_

#include "actor/knowledge.hpp"

class actor_drawing_interface {
    public:
    virtual const grid<knowledge_cell> &get_knowledge_grid() const = 0;
    virtual const character &get_character() const = 0;
};

#endif
