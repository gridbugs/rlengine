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

#include "util/perlin.hpp"
#include "util/arith.hpp"
#include "assert.h"
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

void f(int low, int high, int r, int g, int b) {
    for (int i = low; i < high; ++i) {
        init_color(20 + i, r, g, b);
    }
}

enum class cell_contents {
    TREE,
    DEAD_TREE,
    EMPTY
};

class test_cell : public cell {
    public:
    test_cell(const int j, const int i) : cell(j, i) {};
    int data;
    char ch = ' ';
    double noise;
    int generation = -1;
    test_cell *core = nullptr;
    int classification = -1;

    bool is_water = false;

    int dist_to_water = -1;
    int dist_to_land = -1;

    cell_contents contents = cell_contents::EMPTY;
    cell_contents next_contents = cell_contents::EMPTY;

    double tree_life = 0;
};

test_cell& get_min_border(grid<test_cell> &grid) {
    test_cell *x = nullptr;
    double min_noise = 1;
    grid.for_each_border([&](test_cell &c) {
        if (x == nullptr || c.noise < min_noise) {
            x = &c;
            min_noise = c.noise;
        }
    });

    return *x;
}

class search_cell : public cell {
    public:
    search_cell(const int j, const int i) : cell(j, i) {};
    search_cell *parent = nullptr;
    bool visited = false;
};

class search_node {
    public:
    test_cell *tc;
    double cost;
    int dist;
    search_node(test_cell &tc, double cost, int dist) :
        tc(&tc),
        cost(cost),
        dist(dist)
    {
    }
};

class search_node_comparitor {
    public:
    bool operator()(const search_node &a, const search_node &b) {
        return a.cost > b.cost;
    }
};

bool dijkstra(grid<test_cell> &gr) {
    simple_grid<search_cell> search_grid(gr.width, gr.height);
    test_cell *start = &get_min_border(gr);
    test_cell *end = nullptr;
    std::priority_queue<search_node, std::vector<search_node>, search_node_comparitor> pq;
    pq.push(search_node(*start, 0, 0));
    search_grid.get_cell(start->coord).visited = true;

    std::list<test_cell*> ends;

    while (!pq.empty()) {
        search_node n = pq.top();
        pq.pop();
        test_cell *current = n.tc;
        double cost = n.cost;
        int dist = n.dist;

        if (gr.is_border_cell(*current) && dist > 100) {
            if (end == nullptr) {
                end = current;

                search_cell *tmp = &search_grid.get_cell(end->coord);
                while (tmp) {
                    gr.get_cell(tmp->coord).data = COLOR_BLUE; //COLOR_RED;
                    gr.get_cell(tmp->coord).generation = 0;
                    gr.get_cell(tmp->coord).core = &gr.get_cell(tmp->coord);
                    gr.get_cell(tmp->coord).ch = '~';
                    tmp = tmp->parent;
                }

            } else {
                if (ends.empty() || arithmetic::random_double(0, 1) < 0.01) {
                    ends.push_back(current);
                }
            }
        }

        gr.for_each_neighbour(*current, [&](test_cell &nei) {
            search_cell &sc = search_grid.get_cell(nei.coord);
            if (!sc.visited) {
                sc.parent = &search_grid.get_cell(current->coord);
                sc.visited = true;
                double diff =  (nei.noise - current->noise);
                pq.emplace(nei, cost + diff, dist + 1);
            }
        });

    }

    std::for_each(ends.begin(), ends.end(), [&](test_cell *cptr) {
        search_cell *tmp = &search_grid.get_cell(cptr->coord);
        while (tmp) {
            if (gr.get_cell(tmp->coord).generation != -1) {
                break;
            }
            gr.get_cell(tmp->coord).data = COLOR_BLUE; //COLOR_RED;
            gr.get_cell(tmp->coord).generation = 2;
            gr.get_cell(tmp->coord).core = &gr.get_cell(tmp->coord);
            gr.get_cell(tmp->coord).ch = '~';
            tmp = tmp->parent;
        }
    });

    return true;
}

void grow(grid<test_cell> &gr, int gen) {
    gr.for_each([&](test_cell &c) {
        if (c.generation == gen) {
            gr.for_each_cardinal_neighbour(c, [&](test_cell &nei) {
                if (nei.generation == -1) {
                    test_cell *core = c.core;
                    int next_gen = gen + 1;
                    if (abs(nei.noise - core->noise) < (next_gen * 0.01)) {
                        nei.generation = next_gen;
                        nei.core = core;
                        nei.data = COLOR_BLUE;//COLOR_RED + next_gen;
                        nei.ch = '~';
                    }
                }
            });
        }
    });
}

void flood(grid<test_cell> &gr, test_cell &start, const std::function<void(test_cell&)> &f) {
    std::list<test_cell*> queue;
    queue.push_back(&start);

    simple_grid<generic_cell<bool>> visited(gr.width, gr.height);
    visited.for_each([](generic_cell<bool> &c) {c.data = false;});

    visited.get_cell(start.coord).data = true;

    while (!queue.empty()) {
        test_cell *cptr = queue.front();
        queue.pop_front();

        gr.for_each_neighbour(*cptr, [&](test_cell &nei) {
            if (!visited.get_cell(nei.coord).data && nei.ch == start.ch) {
                visited.get_cell(nei.coord).data = true;
                queue.push_back(&nei);
            }
        });

        f(*cptr);
    }
}

class cell_group {
    public:
    std::list<test_cell*> cells;
};

void classify(grid<test_cell> &gr, std::list<cell_group> &groups) {
    int classification = 0;
    gr.for_each([&](test_cell &c) {
        if (c.classification == -1) {
            groups.emplace_back();
            flood(gr, c, [&](test_cell &fc) {
                fc.classification = classification;
                groups.back().cells.push_back(&fc);
            });
            ++classification;
        }
    });
}

void compute_distances(grid<test_cell> &gr) {
    std::list<test_cell*> water_dist_queue;
    std::list<test_cell*> land_dist_queue;
    gr.for_each([&](test_cell &c) {
        if (c.is_water) {
            c.dist_to_water = 0;
            water_dist_queue.push_back(&c);
        } else {
            c.dist_to_land = 0;
            land_dist_queue.push_back(&c);
        }
    });
    while (!water_dist_queue.empty()) {
        test_cell *c = water_dist_queue.front();
        water_dist_queue.pop_front();
        gr.for_each_neighbour(*c, [&](test_cell &nei) {
            if (nei.dist_to_water == -1) {
                nei.dist_to_water = c->dist_to_water + 1;
                water_dist_queue.push_back(&nei);
            }
        });
    }
    while (!land_dist_queue.empty()) {
        test_cell *c = land_dist_queue.front();
        land_dist_queue.pop_front();
        gr.for_each_neighbour(*c, [&](test_cell &nei) {
            if (nei.dist_to_land == -1) {
                nei.dist_to_land = c->dist_to_land + 1;
                land_dist_queue.push_back(&nei);
            }
        });
    }

}

void place_trees(grid<test_cell> &gr) {
    gr.for_each([](test_cell &c) {
        if (!c.is_water && arithmetic::random_double(0, 1) < 0.1) {
            c.contents = cell_contents::TREE;
            c.ch = '&';
            c.tree_life = static_cast<int>(arithmetic::random_double(0, 50));
        }
    });
}

void tree_tick(grid<test_cell> &gr) {
    gr.for_each([&](test_cell &c) {
        if (c.is_water) {
            return;
        }

        int count = 0;
        gr.for_each_neighbour(c, [&](test_cell &nei) {
            if (nei.contents == cell_contents::TREE) {
                ++count;
            }
            if (nei.contents == cell_contents::DEAD_TREE) {
                if (arithmetic::random_double(0, 1) < 0.3) {
    //                ++count;
                }
            }
        });
        if (c.contents == cell_contents::TREE) {
            c.tree_life -= c.dist_to_water * arithmetic::random_double(0, 1) * 10;
            if (c.tree_life <= 0) {
                if (arithmetic::random_double(0, 1) < 0.05) {
                    c.next_contents = cell_contents::DEAD_TREE;
                } else {
                    c.next_contents = cell_contents::EMPTY;
                }
            } else {
                if (count >= 2 && count <= 3) {
                    c.next_contents = cell_contents::TREE;
                } else {
                    if (arithmetic::random_double(0, 1) < (0.01*c.dist_to_water)) {
//                        c.next_contents = cell_contents::EMPTY;
                        c.tree_life -= c.dist_to_water * 100 * arithmetic::random_double(0, 1);
                    }
                }
            }
        } else if (c.contents == cell_contents::EMPTY) {
            if (count >= 3 && count <= 3) {
                c.tree_life = static_cast<int>(arithmetic::random_double(0, 50));
                c.next_contents = cell_contents::TREE;
            } else {
                if (arithmetic::random_double(0, 1) < (0.005*(40-c.dist_to_water))) {
                    c.tree_life = static_cast<int>(arithmetic::random_double(0, 50));
                    c.next_contents = cell_contents::TREE;
                } else {
                    c.next_contents = cell_contents::EMPTY;
                }
            }
        } else if (c.contents == cell_contents::DEAD_TREE) {
            c.next_contents = cell_contents::EMPTY;
        }
    });

    gr.for_each([](test_cell &c) {
        if (c.is_water) {
            return;
        }
        c.contents = c.next_contents;
    });
}

void update_chars(grid<test_cell> &gr) {
    gr.for_each([](test_cell &c) {
        if (c.is_water) {
            return;
        }
        switch (c.contents) {
        case cell_contents::TREE:
            c.ch = '&';
            c.data = COLOR_GREEN;
            break;
        case cell_contents::EMPTY:
            c.ch = '.';
            c.data = COLOR_GREEN;
            break;
        case cell_contents::DEAD_TREE:
            c.ch = '&';
            c.data = COLOR_YELLOW;
            break;
        default:
            c.ch = '.';
        }
    });
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
//    srand(0);
#if 0
    std::list<std::unique_ptr<active_effect>> test_list;
    cancellable_owner_list<active_effect> test_list_2;
    cancellable_owner_list<active_effect> test_list_3;

    test_list_2 = test_list_3;

    fifo::start();
    curses::simple_start();

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
#endif
#define DRAW
#ifdef DRAW
    char envstr[] = "TERM=xterm-256color";
    putenv(envstr);

    initscr();
    cbreak();
    noecho();
    curs_set(0);
    start_color();

    const int offset = 20;
    for (int i = 0; i < 200; ++i) {
        init_color(offset + i, i*5, i*5, i*5);
        init_pair(offset + i, offset + i, 0);
    }
    for (int i = 0; i < 20; ++i) {
        init_pair(i, i, 0);
    }
#endif
    perlin_grid pg;
    grid<test_cell> dg(140, 40);

    dg.for_each([&](test_cell &c) {
        c.noise = pg.get_noise(c.centre * 0.1);
        c.data = COLOR_GREEN;
        c.ch = '.';
    });

    dijkstra(dg);
    grow(dg, 0);
    grow(dg, 1);
    grow(dg, 2);

    std::list<cell_group> groups;
    classify(dg, groups);

    cell_group *water_group = nullptr;
    std::for_each(groups.begin(), groups.end(), [&](cell_group &g) {
        if (g.cells.front()->ch == '~') {
            water_group = &g;
        }
    });
#ifdef SHOWGROUPS
    dg.for_each([](test_cell &c) {
        c.ch = 'a' + c.classification;
    });
#endif
    
    std::for_each(water_group->cells.begin(), water_group->cells.end(), 
        [&](test_cell *c) {
        c->is_water = true;
        c->ch = '~';
    });
    
    compute_distances(dg);
#ifdef SHOWDISTS
    dg.for_each([](test_cell &c) {
        if (c.is_water) {
            if (c.dist_to_land > 0 && c.dist_to_land <= 26) {
                c.ch = 'a' + c.dist_to_land - 1;
            } else {
                c.ch = '~';
            }
        } else {
            if (c.dist_to_water > 0 && c.dist_to_water <= 26) {
                c.ch = 'a' + c.dist_to_water - 1;
            } else {
                c.ch = '.';
            }
        }
    });
#endif

    place_trees(dg);

    for (int i = 0; i < 200; ++i) {
        tree_tick(dg);
    }
    
    update_chars(dg);

#ifdef DRAW
    dg.for_each([&](test_cell &c) {
        wmove(stdscr, c.y_coord, c.x_coord);
        wattron(stdscr, COLOR_PAIR(c.data));
        waddch(stdscr, c.ch);
        wattroff(stdscr, COLOR_PAIR(c.data));

    });


    wgetch(stdscr);
    endwin();
#endif
    return 0;
}
