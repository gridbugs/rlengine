#ifndef _BEHAVIOUR_HPP_
#define _BEHAVIOUR_HPP_

#include "actor/actor.hpp"
#include "actor/knowledge.hpp"
#include <vector>

class fov_detector;
class character;

class behaviour : public actor {

    protected:
    character &character_;
    fov_detector &fov_;
    grid<knowledge_cell> knowledge_grid_;
    std::vector<game_cell*> visible_cells_;

    public:

    behaviour(character &c, world &w, fov_detector &f) : 
        character_(c),
        fov_(f),
        knowledge_grid_(w.width, w.height)
    {}

    const grid<knowledge_cell> &get_knowledge_grid() const {
        return knowledge_grid_;
    }

    virtual void observe_world(const world &w);

};

#endif
