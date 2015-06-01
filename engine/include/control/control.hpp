#ifndef _CONTROL_HPP_
#define _CONTROL_HPP_

#include "actor/character_actor.hpp"
#include "action/action.hpp"
#include "drawing/drawer.hpp"
#include "debug/fifo.hpp"
#include <array>

using namespace behaviour_tree;

template <typename C, typename W, typename K> class control :
    public character_actor<C, W, K> {

    protected:
    typedef typename character_actor<C, W, K>::world_t world_t;
    typedef typename character_actor<C, W, K>::observer_t observer_t;
    typedef drawer<C, W, K> drawer_t;

    drawer_t &drawer_;
    void draw(world_t &w) const {
        drawer_.draw_world_from_actor(w, *this);
    }
    
    typedef enum {
        INPUT_NONE = 0,
        INPUT_NORTH,
        INPUT_EAST,
        INPUT_SOUTH,
        INPUT_WEST,
        INPUT_NORTHEAST,
        INPUT_SOUTHEAST,
        INPUT_SOUTHWEST,
        INPUT_NORTHWEST,
        N_INPUT
    } input_t;

    virtual input_t get_input() const = 0;

    bool is_direction_input(input_t input) {
        return input >= INPUT_NORTH && input <= INPUT_NORTHWEST;
    }

    direction::direction_t get_input_direction(input_t input) {
        switch(input) {
            case INPUT_NORTH: return direction::north;
            case INPUT_EAST: return direction::east;
            case INPUT_SOUTH: return direction::south;
            case INPUT_WEST: return direction::west;
            case INPUT_NORTHEAST: return direction::northeast;
            case INPUT_SOUTHEAST: return direction::southeast;
            case INPUT_SOUTHWEST: return direction::southwest;
            case INPUT_NORTHWEST: return direction::northwest;
            default: return direction::none;
        }
    }

    bool is_enemy(const C &c) const {
        return true;
    }
    bool is_enemy_in_melee_range_in_direction(world_t &w, direction::direction_t d) {
        return is_enemy_within_distance_in_direction(w, this->character_.get_melee_range(), d);
    }
    bool is_enemy_within_distance_in_direction(world_t &w, int distance, direction::direction_t d) const {

        if (distance == 0) {
            return false;
        }
        const grid<K> &map = this->get_current_knowledge_grid();

        return map.template with_neighbour<bool, false>(map.get_cell(this->character_.coord), d, [&](const K& neighbour) {
            W &world_neighbour = this->get_current_grid(w).get_cell(neighbour.coord);
            if (world_neighbour.is_solid() || world_neighbour.is_opaque()) {
                return false;
            }

            bool contains_enemy = false;
            neighbour.for_each_character([&](const C &c) {
                contains_enemy = contains_enemy || this->is_enemy(c);
            });

            if (contains_enemy) {
                return true;
            }
            return is_enemy_within_distance_in_direction(w, distance - 1, d);
        });
    }

    int act(world_t &w) {

        draw(w);

        timed_result r = seq() && with(get_input(), [&](input_t input) {
            return seq() && [&](){return input != INPUT_NONE;} && [&](){
                return sel() || [&](){
                    return seq() && std::bind(&control::is_direction_input, this, input) &&
                           with(get_input_direction(input), [&](direction::direction_t dir) {
                        return sel() || [&]() {
                            return seq() && std::bind(&control::is_enemy_in_melee_range_in_direction,
                                                      this, std::ref(w), dir) &&
                                   attack_in_direction_action<C, W>(w, this->character_, dir);
                        } || with(move_in_direction_action<C, W>(w, this->character_, dir), 
                                  [&](const move_in_direction_action<C, W> &a) {
                            return seq() && std::bind(&move_in_direction_action<C, W>::is_possible, a) &&
                                   std::bind(&move_in_direction_action<C, W>::is_safe, a) && a;

                        });
                    });
                };
            };
        });

        return r.time;
    }

    bool can_act() const {return true;}

    public:
    control(C &c, world_t &w, observer_t &o, drawer_t &d) :
        character_actor<C, W, K>(c, w, o),
        drawer_(d)
    {}
};

#endif
