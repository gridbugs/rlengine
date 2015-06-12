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
    basic_character(const world_dimensions &w, const vec2<int> &v) :
        character(w, v, 'b', PAIR_RED, PAIR_DARK_RED, 10)
    {}
};

class test {
    bool is_cancelled() {return false;}
};

int main(int argc, char *argv[]) {
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

    schedule<world> s;
    s.register_callback(a0, 0);
    s.register_callback(a1, 1);
    s.register_callback(a2, 2);
    s.register_callback(a3, 3);
    s.register_callback(a4, 4);

    s.run_until_empty(w);

    cancellable_owner_list<test> ol0;
    cancellable_owner_list<test> ol1;
    cancellable_watcher_list<test> wl0;

    ol0.add_watcher(wl0);
    ol1.add_watcher(wl0);

    curses::simple_stop();
    fifo::stop();
    return 0;
}
