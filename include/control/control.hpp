#ifndef _CONTROL_HPP_
#define _CONTROL_HPP_

#include "actor/character_actor.hpp"
#include "drawing/drawer.hpp"
#include "debug/fifo.hpp"
#include "io/curses.hpp"
#include <array>

class control : public character_actor {
    protected:

    drawer &drawer_;
    void draw(world &w) const {
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

    bool is_enemy(const character_image &c) const {
        return true;
    }
    bool is_enemy_in_melee_range_in_direction(world &w, direction::direction_t d) {
        return is_enemy_within_distance_in_direction(w, this->character_.get_melee_range(), d);
    }
    bool is_enemy_within_distance_in_direction(world &w, int distance, direction::direction_t d) const {

        if (distance == 0) {
            return false;
        }
        const grid<knowledge_cell> &map = this->get_current_knowledge_grid();

        return map.with_neighbour<bool, false>(map.get_cell(this->character_.coord), d, [&](const knowledge_cell& neighbour) {
            world_cell &world_neighbour = this->get_current_grid(w).get_cell(neighbour.coord);
            if (world_neighbour.is_solid() || world_neighbour.is_opaque()) {
                return false;
            }

            bool contains_enemy = false;
            neighbour.for_each_character([&](const character_image &c) {
                contains_enemy = contains_enemy || (c.current_hit_points > 0 && this->is_enemy(c));
            });

            if (contains_enemy) {
                return true;
            }
            return is_enemy_within_distance_in_direction(w, distance - 1, d);
        });
    }

    void act(world &w) {

        draw(w);

        input_t input = get_input();

        if (input == INPUT_NONE) {
            return;
        }

        if (is_direction_input(input)) {
            direction::direction_t d = get_input_direction(input);

            if (is_enemy_in_melee_range_in_direction(w, d)) {
                w.register_transaction<try_attack_transaction>(this->character_, d);
                return;
            } else {
                w.register_transaction<try_move_transaction>(this->character_, d);
                return;
            }
        }
    }

    bool can_act() const {return true;}

    public:
    control(character &c, observer &o, drawer &d) :
        character_actor(c, o),
        drawer_(d)
    {}
};

#endif
