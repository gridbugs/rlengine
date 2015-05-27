#ifndef _KNOWLEDGE_HPP_
#define _KNOWLEDGE_HPP_

#include "world/grid.hpp"
#include "world/cell.hpp"
#include "util/arith.hpp"

#include <list>

#define REMEMBERED  BIT(0)
#define VISIBLE     BIT(1)

template <typename C, typename W>
class knowledge_cell : public cell {

    protected:
    unsigned int knowledge_;

    public:
    std::list<C> characters;
    
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

    void see_character(const C &c) {
        characters.push_back(c);
    }

    void unsee_characters() {
        characters.clear();
    }

    bool contains_character() const {
        return !characters.empty();
    }
};

#endif
