#include <world/world.hpp>
#include <samples/conway_generator.hpp>
#include <io/curses.hpp>
#include <samples/curses_drawer.hpp>
#include <actor/player_actor.hpp>
#include <ncurses.h>
#include <observer/shadow_cast_fov.hpp>
#include <observer/omniscient_fov.hpp>

class demo_character : public character {
    public:
    int hit_points;
    char char_to_draw;
    pair_t col_pair;
    demo_character(const vec2<int> &v, int hp, char ch, pair_t col_pair) : 
        character(v),
        hit_points(hp),
        char_to_draw(ch),
        col_pair(col_pair)
    {}

    void take_damage(int d) {
        hit_points -= d;
    }
};

typedef knowledge_cell<demo_character, game_cell> kcell_t;

class demo_drawer : public curses_drawer<demo_character, game_cell, kcell_t> {
    public:
    demo_drawer() : curses_drawer() {}

    protected:

    void draw_cell_from_actor(const game_cell &cell, const kcell_t &kcell, const demo_character &ctr) {
        wmove(curses::game_window, cell.y_coord, cell.x_coord);

        char ch;
        pair_t pair = PAIR_UNKNOWN;
        bool bold = false;

        if (kcell.contains_character()) {
            ch = kcell.characters.front().char_to_draw;
            bold = true;
        } else if (cell.is_opaque()) {
            ch = '#';
        } else {
            ch = '.';
        }
        if (kcell.contains_character()) {
            pair = kcell.characters.front().col_pair; 
        } else if (kcell.is_visible()) {
            pair = PAIR_VISIBLE;
        } else if (kcell.is_remembered()) {
            pair = PAIR_REMEMBERED;
        } else {
            pair = PAIR_UNKNOWN;
        }

        if (bold) {
            wattron(curses::game_window, A_BOLD);
        }
        wattron(curses::game_window, COLOR_PAIR(pair));
        waddch(curses::game_window, ch);
        wattroff(curses::game_window, COLOR_PAIR(pair));
        if (bold) {
            wattroff(curses::game_window, A_BOLD);
        }
    }
};

int main(int argc, char *argv[]) {
    curses::simple_start();
    srand(2);

    world<demo_character, game_cell> w(100, 40);
    conway_generator<demo_character, game_cell> gen;
    gen.generate(w);
    shadow_cast_fov<demo_character, game_cell, kcell_t> fov;
    //omniscient_fov<demo_character, game_cell, kcell_t> fov;
    demo_drawer dr;
    schedule<world<demo_character, game_cell>> s;
    
    w.characters.push_back(demo_character(w.get_random_empty_cell(0).coord, 10, '@', PAIR_WHITE));
    
    w.characters.push_back(demo_character(w.get_random_empty_cell(0).coord, 10, 'b', PAIR_RED));
    w.characters.push_back(demo_character(w.get_random_empty_cell(0).coord, 10, 'b', PAIR_RED));
    w.characters.push_back(demo_character(w.get_random_empty_cell(0).coord, 10, 'b', PAIR_RED));
    w.characters.push_back(demo_character(w.get_random_empty_cell(0).coord, 10, 'b', PAIR_RED));
    
    demo_character &player = w.characters[0];
    
    player_actor<demo_character, game_cell, kcell_t>  actor(player, w, fov, dr);
    actor.init_dvorak();


    s.register_callback(actor, 0);


    s.run_until_empty(w);
    
    curses::simple_stop();

    return 0;
}
