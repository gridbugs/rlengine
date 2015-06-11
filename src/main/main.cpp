#include "world/world.hpp"
#include "actor/character_actor.hpp"
#include "control/control.hpp"
#include "control/curses_control.hpp"
#include "world/conway_generator.hpp"
#include "world/border_generator.hpp"
#include "observer/shadow_cast_fov.hpp"
#include "actor/always_move_left.hpp"
#include "drawing/curses_drawer.hpp"

#include "debug/fifo.hpp"
#include "io/curses.hpp"

#include <memory>

class basic_character : public character {
    public:
    basic_character(const vec2<int> &v) :
        character(v, 'b', PAIR_RED, PAIR_DARK_RED)
    {}
};

int main(int argc, char *argv[]) {
    fifo::start();
    curses::simple_start();
    srand(2);

    world w(100, 40);
    std::make_unique<conway_generator>()->generate(w);

    shadow_cast_fov fov;
    curses_drawer dr;

    w.characters.push_back(std::make_unique<character>(w.get_random_empty_cell(0).coord));
    w.characters.push_back(std::make_unique<basic_character>(w.get_random_empty_cell(0).coord));
    w.characters.push_back(std::make_unique<basic_character>(w.get_random_empty_cell(0).coord));
    w.characters.push_back(std::make_unique<basic_character>(w.get_random_empty_cell(0).coord));
    w.characters.push_back(std::make_unique<basic_character>(w.get_random_empty_cell(0).coord));

    curses_control a0(*w.characters[0], w, fov, dr);
    always_move_left a1(*w.characters[1], w, fov);
    always_move_left a2(*w.characters[2], w, fov);
    always_move_left a3(*w.characters[3], w, fov);
    always_move_left a4(*w.characters[4], w, fov);

    a0.init_dvorak();

    schedule<world> s;
    s.register_callback(a0, 0);
    s.register_callback(a1, 1);
    s.register_callback(a2, 2);
    s.register_callback(a3, 3);
    s.register_callback(a4, 4);

    s.run_until_empty(w);

    curses::simple_stop();
    fifo::stop();
    return 0;
}
