#ifndef _WORLD_HPP_
#define _WORLD_HPP_

#include "world/grid.hpp"
#include "geometry/vec2.hpp"
#include "character/character.hpp"
#include "transaction/transaction_queue.hpp"
#include <vector>
#include <memory>



template <typename C, typename W, typename T> class world {
    public:
    
    std::vector<grid<W>> maps;
    std::vector<std::unique_ptr<C>> characters;
    typedef transaction_queue<T, C, W> transaction_queue_t;
    transaction_queue_t transactions;
    typedef typename transaction_queue_t::transaction_t transaction_t;
    typedef typename transaction_queue_t::transaction_ptr_t transaction_ptr_t;

    const int width;
    const int height;
    world(const int width, const int height) : 
        width(width),
        height(height)
    {
        maps.push_back(grid<W>(width, height));
    }

    void with_character_at_coord(int level, vec2<int> coord, const std::function<void(C&)> &f) {
        std::for_each(characters.begin(), characters.end(), [&](std::unique_ptr<C> &c) {
            if (c->level_index == level && c->coord == coord) {
                f(*c);
            }
        });
    }

    W& get_random_empty_cell(int index) {
        for (;;) {
            int x = rand() % width;
            int y = rand() % height;
            W& ret = maps[index].get_cell(x, y);
            if (!ret.is_solid()) {
                return ret;
            }
        }
        return maps[index][0][0];
    }

    void move_character(C &c, vec2<int> coord) {
        if (!maps[c.level_index].get_cell(coord).is_solid()) {
            c.coord = coord;
        }
    }

};

#endif
