#ifndef _CHARACTER_HPP_
#define _CHARACTER_HPP_

#include "geometry/vec2.hpp"
#include "geometry/direction.hpp"
#include "drawing/curses_col.hpp"
#include "character/character_image.hpp"
#include "knowledge/knowledge.hpp"
#include "effect/active_effect.hpp"
#include "util/cancellable.hpp"

#include <algorithm>

class world;

class character {
    protected:
    world &world_;

    public:
    vec2<int> coord;
    char char_repr = '@';
    pair_t col_pair_visible = PAIR_WHITE;
    pair_t col_pair_remembered = PAIR_WHITE;
    int max_hit_points = 10;
    int current_hit_points = 10;
    int level_index = 0;

    std::vector<knowledge_grid> knowledge_grids;

    cancellable_owner_list<active_effect> active_effects;

    protected:
    void init_knowledge_grids();

    public:
    character(world &w, const vec2<int> &v) :
        world_(w),
        coord(v)
    {
        init_knowledge_grids();
    }

    character(world &w, const vec2<int> &v, char c, pair_t vp, pair_t rp,
                int hit_points) :
        world_(w),
        coord(v),
        char_repr(c),
        col_pair_visible(vp),
        col_pair_remembered(rp),
        max_hit_points(hit_points),
        current_hit_points(hit_points)
    {
        init_knowledge_grids();
    }

    world& get_world() {return world_;}

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
