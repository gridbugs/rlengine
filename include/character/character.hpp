#ifndef _CHARACTER_HPP_
#define _CHARACTER_HPP_

#include "world/world_dimensions.hpp"
#include "geometry/vec2.hpp"
#include "geometry/direction.hpp"
#include "drawing/curses_col.hpp"
#include "character/character_image.hpp"
#include "knowledge/knowledge.hpp"

#include <algorithm>

class character {
    public:
    vec2<int> coord;
    char char_repr = '@';
    pair_t col_pair_visible = PAIR_WHITE;
    pair_t col_pair_remembered = PAIR_WHITE;
    int max_hit_points = 10;
    int current_hit_points = 10;
    int level_index = 0;
    
    std::vector<knowledge_grid> knowledge_grids;

    protected:
    void init_knowledge_grids(int x, int y, int z) {
        for (int i = 0; i < z; ++i) {
            knowledge_grids.push_back(knowledge_grid(x, y));
        }
    }

    public:
    character(const world_dimensions &wd, const vec2<int> &v) :
        coord(v)
    {
        init_knowledge_grids(wd.get_width(), wd.get_height(), wd.get_depth());
    }

    character(const world_dimensions &wd, const vec2<int> &v, char c, pair_t vp, pair_t rp,
                int hit_points) :
        coord(v),
        char_repr(c),
        col_pair_visible(vp),
        col_pair_remembered(rp),
        max_hit_points(hit_points),
        current_hit_points(hit_points)
    {
        init_knowledge_grids(wd.get_width(), wd.get_height(), wd.get_depth());
    }

    virtual int get_move_time() const {return 1;}
    virtual int get_melee_range() const {return 1;}
    virtual int get_melee_damage() const {return 1;}

    virtual char get_char() const {return char_repr;}
    virtual pair_t get_col_pair_visible() const {return col_pair_visible;}
    virtual pair_t get_col_pair_remembered() const {return col_pair_remembered;}

    character_image get_image() const {
        return character_image(coord, char_repr, col_pair_visible, col_pair_remembered, 
                max_hit_points, current_hit_points, this);
    }

    knowledge_grid &get_current_knowledge_grid() {
        return knowledge_grids[level_index];
    }

    const knowledge_grid &get_current_knowledge_grid() const {
        return knowledge_grids[level_index];
    }

    void take_damage(int d) {
        current_hit_points = std::max(current_hit_points - d, 0);
    }

    bool is_alive() {
        return current_hit_points > 0;
    }
};

#endif
