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

#include "util/cancellable.hpp"

#include <memory>

class basic_character : public character {
    public:
    basic_character(world &w, const vec2<int> &v) :
        character(w, v, 'b', PAIR_RED, PAIR_DARK_RED, 10)
    {}
};

class test {
    bool is_cancelled() {return false;}
};

int main(int argc, char *argv[]) {

    std::list<std::unique_ptr<active_effect>> test_list;
    cancellable_owner_list<active_effect> test_list_2;
    cancellable_owner_list<active_effect> test_list_3;

    test_list_2 = test_list_3;

    fifo::start();
    curses::simple_start();
    srand(2);

    world w(100, 40);
    std::make_unique<conway_generator>()->generate(w);

    shadow_cast_fov fov;
    curses_drawer dr;

    w.characters.push_back(std::make_unique<character>(w, w.get_random_empty_cell(0).coord));
    w.characters.push_back(std::make_unique<basic_character>(w, w.get_random_empty_cell(0).coord));
    w.characters.push_back(std::make_unique<basic_character>(w, w.get_random_empty_cell(0).coord));
    w.characters.push_back(std::make_unique<basic_character>(w, w.get_random_empty_cell(0).coord));
    w.characters.push_back(std::make_unique<basic_character>(w, w.get_random_empty_cell(0).coord));

    curses_control a0(*w.characters[0], fov, dr);
    always_move_left a1(*w.characters[1], fov);
    always_move_left a2(*w.characters[2], fov);
    always_move_left a3(*w.characters[3], fov);
    always_move_left a4(*w.characters[4], fov);

    a0.init_dvorak();

    w.get_schedule().register_callback(a0, 0);
    w.get_schedule().register_callback(a1, 1);
    w.get_schedule().register_callback(a2, 2);
    w.get_schedule().register_callback(a3, 3);
    w.get_schedule().register_callback(a4, 4);

    w.get_schedule().run_until_empty();

    curses::simple_stop();
    fifo::stop();
    return 0;
}
