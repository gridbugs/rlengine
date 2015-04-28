#ifndef _BEHAVIOUR_HPP_
#define _BEHAVIOUR_HPP_

#include "actor/actor.hpp"

class character;

class behaviour : public actor {

    protected:
    character *character_;

    public:

    virtual bool register_character(character &c) {
        if (character_ == nullptr) {
            character_ = &c;
            return true;
        }
        return false;
    }
    
    virtual bool deregister_character(character &c) {
        if (character_ == &c) {
            character_ = nullptr;
            return true;
        }
        return false;
    }


};

#endif
