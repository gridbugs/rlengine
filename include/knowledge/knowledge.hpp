#ifndef _KNOWLEDGE_HPP_
#define _KNOWLEDGE_HPP_

#include "world/grid.hpp"
#include "world/cell.hpp"
#include "util/arith.hpp"
#include "character/character_image.hpp"

#include <list>

#define REMEMBERED  BIT(0)
#define VISIBLE     BIT(1)

class character;

class knowledge_cell : public cell {

    protected:
    unsigned int knowledge_;

    public:
    std::list<character_image> characters;
    
    knowledge_cell(const int j, const int i) : 
        cell(j, i),
        knowledge_(0)
    {}

    /* Determine how a cell is known */
    bool is_unknown() const {
        return knowledge_ == 0;
    }
    bool is_remembered() const {
        return knowledge_ & REMEMBERED;
    }

    bool is_visible() const {
        return knowledge_ & VISIBLE;
    }
    void see() {
        knowledge_ |= (VISIBLE | REMEMBERED);
    }
    void partially_see() {
        see();
    }
    void unsee() {
        knowledge_ &= ~VISIBLE;
    }

    void see_character(const character &c);

    void unsee_characters() {
        characters.clear();
    }

    bool contains_character() const {
        return !characters.empty();
    }
    
    void for_each_character(const std::function<void(character_image&)> &f) {
        std::for_each(characters.begin(), characters.end(), f);
    }
    
    void for_each_character(const std::function<void(const character_image&)> &f) const {
        std::for_each(characters.begin(), characters.end(), f);
    }
};

#endif
