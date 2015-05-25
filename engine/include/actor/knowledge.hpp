#ifndef _KNOWLEDGE_HPP_
#define _KNOWLEDGE_HPP_

#include "world/grid.hpp"
#include "world/cell.hpp"

class knowledge_cell : public cell {

    protected:
    unsigned int knowledge_;

    public:
    knowledge_cell(const int j, const int i) : 
        cell(j, i),
        knowledge_(0)
    {}

    /* Cell is not unknown */
    bool is_known() const;

    /* Determine how a cell is known */
    bool is_unknown() const;
    bool is_remembered() const;
    bool is_visible() const;
    bool is_magic() const;
    
    /* Gaining knowledge */
    void see();
    void memorize();
    void magically_reveal();

    /* Losing knowledge */
    void unsee();
    void forget();
    void unmagic();
};

#endif
