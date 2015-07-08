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
#include <algorithm>
#include <limits>

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

class cell_group;

class test_cell : public cell {
    public:
    test_cell(const int j, const int i) : cell(j, i) {};
    int data;
    char ch = ' ';
    double noise;
    double noise_old;
    int generation = -1;
    test_cell *core = nullptr;
    int classification = -1;
    int classification2 = -1;
    int reclas = -1;

    bool is_water = false;
    bool is_water_edge = false;
    bool is_water_hard_edge = false;
    bool is_open = true;
    bool is_enclosed = false;
    bool is_wall = false;
    bool is_hard_wall = false;
    bool is_water_end = false;

    int dist_to_water = -1;
    int dist_to_land = -1;

    cell_contents contents = cell_contents::EMPTY;
    cell_contents next_contents = cell_contents::EMPTY;

    double tree_life = 0;

    int space_up;
    int space_down;
    int space_left;
    int space_right;

    int expand_dist = -1;

    bool is_bridge_candidate = false;
    direction::direction_t bridge_direction = direction::none;
    test_cell *bridge_far_side = nullptr;
    bool is_bridge = false;

    bool is_entrance_candidate = false;
    direction::direction_t entrance_direction;
    test_cell *entrance_outside_cell = nullptr;
    test_cell *entrance_final_outside_cell = nullptr;
    bool is_entrance = false;

    cell_group *group = nullptr;
    bool start_candidate = true;
    int start_distance = 0;
};



void make_bridge(grid<test_cell> &gr, test_cell &c) {
    test_cell *tmp = &c;
    while (tmp->is_water) {
        tmp->is_bridge = true;
        tmp = gr.get_neighbour(*tmp, c.bridge_direction);
    }
}

test_cell& get_min_border(grid<test_cell> &grid) {
    test_cell *x = nullptr;
    double min_noise = 1;
    grid.for_each([&](test_cell &c) {
        if (grid.get_distance_to_border(c) == 5) {
            if (x == nullptr || c.noise < min_noise) {
                x = &c;
                min_noise = c.noise;
            }
        }
    });

    return *x;
}

class search_cell : public cell {
    public:
    search_cell(const int j, const int i) : cell(j, i) {};
    search_cell *parent = nullptr;
    bool visited = false;
    double cost = std::numeric_limits<double>::infinity();
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

bool river(grid<test_cell> &gr, test_cell &start_cell, 
    std::function<double(test_cell &current, test_cell &neighbour, direction::direction_t dir)> step_fn,
    std::function<bool(test_cell &current)> predicate,
    std::function<void(test_cell &river_cell)> river_fn){
    test_cell *start = &start_cell;
    simple_grid<search_cell> search_grid(gr.width, gr.height);
    std::priority_queue<search_node, std::vector<search_node>, search_node_comparitor> pq;
    pq.push(search_node(*start, 0, 0));
    bool found = false;
    test_cell *end = nullptr;

    while (!pq.empty()) {
        search_node n = pq.top();
        pq.pop();
        test_cell *current = n.tc;
        search_cell &c = search_grid.get_cell(current->coord);
        if (c.visited) {
            continue;
        }
        c.visited = true;

        if (predicate(*current)) {
            found = true;
            end = current;
            break;
        }
        std::for_each(direction::all_directions.begin(), direction::all_directions.end(),
            [&](direction::direction_t d) {

            test_cell *nei = gr.get_neighbour(*current, d);
            if (nei == nullptr) {
                return;
            }
            search_cell &sc = search_grid.get_cell(nei->coord);
            if (sc.visited) {
                return;
            }
            double step_cost;
            double basic_cost;
            if (direction::is_ordinal(d)) {
                basic_cost = 1.414;
            } else {
                basic_cost = 1;
            }
            step_cost = step_fn(*current, *nei, d);

            double cost = n.cost + step_cost + basic_cost;
            
            if (cost < sc.cost) {
                sc.cost = cost;
                sc.parent = &c;
                pq.emplace(*nei, cost, 0);
            }
        });
    }

    if (!found) {
        return false;
    }

    search_cell *tmp = &search_grid.get_cell(end->coord);
    while (tmp != nullptr) {
        river_fn(gr.get_cell(tmp->coord));
        tmp = tmp->parent;
    }


    return found;
}

test_cell *get_random_border_spaced(grid<test_cell> &gr, int space, double min, double max) {
    std::vector<test_cell*> border;
    gr.for_each_border([&](test_cell &c) {
        if (c.noise > min && c.noise < max) {
            border.push_back(&c);
        }
    });

    test_cell *c;
    int i = 0;
    while (i < 100) {
        c = border[arithmetic::random_int(0, border.size())];
        if (c->start_candidate) {
            break;
        }
        ++i;
    }

    if (i == 100) {
        return nullptr;
    }
    
    std::list<test_cell*> queue;
    queue.push_back(c);
    while (!queue.empty()) {
        test_cell &next = *(queue.front());
        if (next.start_distance >= space) {
            break;
        }
        queue.pop_front();
        gr.for_each_neighbour(next, [&](test_cell& nei) {
            if (nei.start_distance == 0) {
                nei.start_distance = next.start_distance + 1;
                nei.start_candidate = false;
                nei.ch = ',';
                queue.push_back(&nei);
            }
        });
    }
    
    return c;
}

bool plot_rivers(grid<test_cell> &gr) {

    test_cell* start = nullptr;
    test_cell* end = nullptr;

    start = get_random_border_spaced(gr, 50, -0.1, 0.1);
    end = get_random_border_spaced(gr, 50, -0.1, 0.1);

    assert(start && end);
    start->ch = 's';
    end->ch = 'e';

    bool found = false;

    found = river(gr, *start,
        [&](test_cell &current, test_cell &neighbour, direction::direction_t dir) {
            double step_cost = (neighbour.noise - current.noise) * 1000;
            int dist_to_edge = gr.get_distance_to_edge(neighbour);
            if (dist_to_edge < 10 && neighbour.coord.distance(end->coord) > 8) {
                int x = 10 - dist_to_edge;
                step_cost += x * x;
            }
            return step_cost;
        },
        [&](test_cell &current) {
            return &current == end;
        },
        [&](test_cell &river_cell) {
            river_cell.ch = '~';
            river_cell.generation = 0;
        }
    );

    assert(found);

    gr.for_each([&](test_cell &c) {
        c.start_candidate = true;
    });
    for (int i = 0; i < arithmetic::random_int(1, 5); ++i) {
        test_cell *c = get_random_border_spaced(gr, 0, -0.1, 0.1);
        if (c) {
        
            river(gr, *c,
                [&](test_cell &current, test_cell &neighbour, direction::direction_t dir) {
                    double step_cost = (neighbour.noise - current.noise) * 1000;
                    int dist_to_edge = gr.get_distance_to_edge(neighbour);
                    if (dist_to_edge < 10 && neighbour.coord.distance(end->coord) > 8) {
                        int x = 10 - dist_to_edge;
                        step_cost += x * x;
                    }
                    return step_cost;
                },
                [&](test_cell &current) {
                    return current.ch == '~';
                },
                [&](test_cell &river_cell) {
                    river_cell.ch = '~';
                    river_cell.generation = 1;
                }
            );       
        
        
        }
    }
    return true;
}


bool dijkstra(grid<test_cell> &gr) {

    gr.for_each([&](test_cell &c) {
        if (gr.get_distance_to_border(c) < 5) {
            c.noise_old = c.noise;
            c.noise = 100;
        }
    });

    simple_grid<search_cell> search_grid(gr.width, gr.height);
    test_cell *start = &get_min_border(gr);
    start->is_water_end = true;
    test_cell *end = nullptr;
    std::priority_queue<search_node, std::vector<search_node>, search_node_comparitor> pq;
    pq.push(search_node(*start, 0, 0));
    search_grid.get_cell(start->coord).visited = true;

    std::list<test_cell*> ends;
    bool ret = false;

    while (!pq.empty()) {
        search_node n = pq.top();
        pq.pop();
        test_cell *current = n.tc;
        double cost = n.cost;
        int dist = n.dist;
//        n.visited = true;

        if (gr.get_distance_to_border(*current) == 5 && 
            current->coord.distance(start->coord) >= 100) {
            if (end == nullptr) {
                end = current;
                current->is_water_end = true;

                search_cell *tmp = &search_grid.get_cell(end->coord);
                while (tmp) {
                    gr.get_cell(tmp->coord).data = COLOR_BLUE; //COLOR_RED;
                    gr.get_cell(tmp->coord).generation = 0;
                    gr.get_cell(tmp->coord).core = &gr.get_cell(tmp->coord);
                    gr.get_cell(tmp->coord).ch = '~';
                    tmp = tmp->parent;
                    ret = true;
                }

            } else {
                if (arithmetic::random_double(0, 1) < 0.02)
                     {
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
        int count = 0;
        while (tmp) {
            if (gr.get_cell(tmp->coord).generation != -1) {
                break;
            }
            gr.get_cell(tmp->coord).data = COLOR_BLUE; //COLOR_RED;
            gr.get_cell(tmp->coord).generation = 2;
            gr.get_cell(tmp->coord).core = &gr.get_cell(tmp->coord);
            gr.get_cell(tmp->coord).ch = '~';
            tmp = tmp->parent;
            ++count;
        }

        if (count < 10) {
            tmp = &search_grid.get_cell(cptr->coord);
            while (tmp) {
                if (gr.get_cell(tmp->coord).generation == 0) {
                    break;
                }
                gr.get_cell(tmp->coord).data = COLOR_GREEN;
                gr.get_cell(tmp->coord).generation = -1;
                gr.get_cell(tmp->coord).core = nullptr;
                gr.get_cell(tmp->coord).ch = 'x';
                tmp = tmp->parent;
            }
            cptr->is_water_end = false;
        } else {
            cptr->is_water_end = true;
        }
    });
    
    gr.for_each([&](test_cell &c) {
        if (gr.get_distance_to_border(c) < 5) {
            c.noise = c.noise_old;
        }
    });


    return ret;
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

void continue_water_to_border(grid<test_cell> &gr, test_cell &end_point) {

    test_cell *tmp = &end_point;
    test_cell *prev = tmp;

    while (gr.get_distance_to_border(*tmp) == 5) {
        prev = tmp;
        gr.for_each_neighbour(*tmp, [&](test_cell &nei) {
            if (nei.ch == '~') {
                tmp->ch = 'x';
                tmp = &nei;
            }
        });
    }

    tmp = prev;

    direction::direction_t dir = direction::none;
    std::for_each(direction::all_directions.begin(), direction::all_directions.end(),
                  [&](direction::direction_t d) {
        test_cell *nei = gr.get_neighbour(*tmp, d);
        if (nei && nei->ch == '~') {
            dir = direction::get_opposite(d);
        }
    });
    
    prev = tmp;
//    assert(prev->generation != -1);
    tmp = gr.get_neighbour(*tmp, dir);
    while (tmp && tmp->ch != '~') {
        tmp->ch = '~';
        tmp = gr.get_neighbour(*tmp, dir);
//        tmp->generation = prev->generation;
    }
    prev->ch = '~';
}

void continue_water_to_border(grid<test_cell> &gr) {
    std::list<test_cell*> ends;
    gr.for_each([&](test_cell &c) {
        if (c.is_water_end) {
            continue_water_to_border(gr, c);
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

void flood2(grid<test_cell> &gr, test_cell &start, const std::function<void(test_cell&)> &f) {
    std::list<test_cell*> queue;
    queue.push_back(&start);

    simple_grid<generic_cell<bool>> visited(gr.width, gr.height);
    visited.for_each([](generic_cell<bool> &c) {c.data = false;});

    visited.get_cell(start.coord).data = true;

    while (!queue.empty()) {
        test_cell *cptr = queue.front();
        queue.pop_front();

        gr.for_each_neighbour(*cptr, [&](test_cell &nei) {
            if (!visited.get_cell(nei.coord).data && !nei.is_water && !nei.is_enclosed && !nei.is_wall) {
                visited.get_cell(nei.coord).data = true;
                queue.push_back(&nei);
            }
        });

        f(*cptr);
    }
}

void flood_enclosure(grid<test_cell> &gr, test_cell &start, const std::function<void(test_cell&)> &f) {
    std::list<test_cell*> queue;
    queue.push_back(&start);

    simple_grid<generic_cell<bool>> visited(gr.width, gr.height);
    visited.for_each([](generic_cell<bool> &c) {c.data = false;});

    visited.get_cell(start.coord).data = true;

    while (!queue.empty()) {
        test_cell *cptr = queue.front();
        queue.pop_front();

        gr.for_each_cardinal_neighbour(*cptr, [&](test_cell &nei) {
            if (!visited.get_cell(nei.coord).data && nei.is_enclosed == start.is_enclosed &&
                    nei.classification == start.classification) {
                visited.get_cell(nei.coord).data = true;
                queue.push_back(&nei);
            }
        });

        f(*cptr);
    }
}

void mark_water_edge(grid<test_cell> &gr) {
    gr.for_each([&](test_cell &c) {
        if (c.is_water) {
            gr.for_each_neighbour(c, [&](test_cell &nei) {
                if (!nei.is_water) {
                    c.is_water_edge = true;
                }
            });
            gr.for_each_cardinal_neighbour(c, [&](test_cell &nei) {
                if (!nei.is_water) {
                    c.is_water_hard_edge = true;
                }
            });
            if (c.is_water_hard_edge) {
                c.ch = '~';
            } else if (c.is_water_edge) {
                c.ch = '~';
            }
        }
    });
}

class cell_group {
    public:
    unsigned int classification;
    std::list<test_cell*> cells;
    std::vector<bool> connectable_groups;
    std::vector<std::list<test_cell*>> potential_bridges;
    bool is_border_adjacent = false;
    bool contains_enclosure = false;
    bool is_enclosure_adjacent = false;
};

void classify(grid<test_cell> &gr, std::list<cell_group> &groups) {
    int classification = 0;
    gr.for_each([&](test_cell &c) {
        if (c.classification == -1) {
            groups.emplace_back();
            groups.back().classification = classification;
            flood(gr, c, [&](test_cell &fc) {
                fc.classification = classification;
                groups.back().cells.push_back(&fc);
                fc.group = &groups.back();
            });
            ++classification;
        }
    });

    std::for_each(groups.begin(), groups.end(), [&](cell_group &gi) {
        std::for_each(groups.begin(), groups.end(), [&](cell_group &gj) {
            gi.connectable_groups.push_back(false);
            gi.potential_bridges.emplace_back();
        });
    });
}

void reclassify(grid<test_cell> &gr, std::list<cell_group> &groups) {
    int classification = 0;
    gr.for_each([&](test_cell &c) {
        if (c.reclas == -1 && !c.is_water && !c.is_enclosed && !c.is_wall) {
            groups.emplace_back();
            groups.back().classification = classification;
            flood2(gr, c, [&](test_cell &fc) {
                fc.reclas = classification;
                groups.back().cells.push_back(&fc);
                fc.group = &groups.back();
            });
            ++classification;
        }
    });

    std::for_each(groups.begin(), groups.end(), [&](cell_group &gi) {

        gi.connectable_groups.clear();
        gi.potential_bridges.clear();

        std::for_each(groups.begin(), groups.end(), [&](cell_group &gj) {
            gi.connectable_groups.push_back(false);
            gi.potential_bridges.emplace_back();
        });
    });
}

void classify_enclosure_group(grid<test_cell> &gr, cell_group &eg, std::list<cell_group> &groups) {
    int clas = groups.size();
    std::for_each(eg.cells.begin(), eg.cells.end(), [&](test_cell *c) {
        if (c->classification2 == -1) {
            groups.emplace_back();
            groups.back().classification = clas;
            groups.back().is_enclosure_adjacent = true;
            flood_enclosure(gr, *c, [&](test_cell &fc) {
                fc.classification2 = clas;
                groups.back().cells.push_back(&fc);
                fc.group = &groups.back();
            });
            ++clas;
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
        if (c.is_open && arithmetic::random_double(0, 1) < 0.1) {
            c.contents = cell_contents::TREE;
            c.ch = '&';
            c.tree_life = static_cast<int>(arithmetic::random_double(0, 50));
        }
    });
}

void tree_tick(grid<test_cell> &gr) {
    gr.for_each([&](test_cell &c) {
        if (!c.is_open) {
            return;
        }

        int count = 0;
        gr.for_each_neighbour(c, [&](test_cell &nei) {
            if (nei.contents == cell_contents::TREE) {
                ++count;
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
        if (!c.is_open) {
            return;
        }
        c.contents = c.next_contents;
    });
}

void update_chars(grid<test_cell> &gr) {
    gr.for_each([](test_cell &c) {
        if (!c.is_open) {
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

void compute_space(grid<test_cell> &gr) {
    if (gr[0][0].is_water) {
        gr[0][0].space_up = 0;
        gr[0][0].space_left = 0;
    } else {
        gr[0][0].space_up = 1;
        gr[0][0].space_left = 1;
    }
    for (int i = 1; i < gr.width; ++i) {
        if (gr[0][i].is_water) {
            gr[0][i].space_up = 0;
            gr[0][i].space_left = 0;
        } else {
            gr[0][i].space_up = 1;
            gr[0][i].space_left = gr[0][i-1].space_left + 1;
        }
    }
    for (int i = 1; i < gr.height; ++i) {
        if (gr[i][0].is_water) {
            gr[i][0].space_up = 0;
            gr[i][0].space_left = 0;
        } else {
            gr[i][0].space_up = gr[i-1][0].space_up + 1;
            gr[i][0].space_left = 1;
        }
    }
    for (int i = 1; i < gr.height; ++i) {
        for (int j = 1; j < gr.width; ++j) {
            if (gr[i][j].is_water) {
                gr[i][j].space_up = 0;
                gr[i][j].space_left = 0;
            } else {
                gr[i][j].space_up = gr[i-1][j].space_up + 1;
                gr[i][j].space_left = gr[i][j-1].space_left + 1;
            }
        }
    }

    test_cell &bottom_right = gr[gr.height-1][gr.width-1];
    if (bottom_right.is_water) {
        bottom_right.space_down = 0;
        bottom_right.space_right = 0;
    } else {
        bottom_right.space_down = 1;
        bottom_right.space_right = 1;
    }
    for (int i = gr.width-2; i >= 0; --i) {
        test_cell &c = gr[gr.height-1][i];
        if (c.is_water) {
            c.space_down = 0;
            c.space_right = 0;
        } else {
            c.space_down = 1;
            c.space_right = gr[gr.height-1][i+1].space_right + 1;
        }
    }
    for (int i = gr.height-2; i >= 0; --i) {
        test_cell &c = gr[i][gr.width-1];
        if (c.is_water) {
            c.space_down = 0;
            c.space_right = 0;
        } else {
            c.space_down = gr[i+1][gr.width-1].space_down + 1;
            c.space_right = 1;
        }
    }
    for (int i = gr.height-2; i >= 0; --i) {
        for (int j = gr.width-2; j >= 0; --j) {
            test_cell &c = gr[i][j];
            if (c.is_water) {
                c.space_down = 0;
                c.space_right = 0;
            } else {
                c.space_down = gr[i+1][j].space_down + 1;
                c.space_right = gr[i][j+1].space_right + 1;
            }
        }
    }
}

class rectangle {
    public:
    int width;
    int height;
    vec2<int> topleft;
    vec2<int> bottomright;
    rectangle(int width, int height, const vec2<int> &topleft) :
        width(width),
        height(height),
        topleft(topleft),
        bottomright(topleft + vec2<int>(width - 1, height - 1))
    {}

    void sync() {
        bottomright = topleft + vec2<int>(width - 1, height - 1);
    }

    int get_area() {
        return width * height;
    }

    void shrink(int x) {
        topleft = topleft + vec2<int>(x, x);
        width -= x*2;
        height -= x*2;
        sync();
    }

    void for_each(grid<test_cell> &gr, std::function<void(test_cell&)> f) {
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                f(gr.get_cell(topleft + vec2<int>(j, i)));
            }
        }
    }
    void for_each_border(grid<test_cell> &gr, std::function<void(test_cell&)> f) {
        for (int i = 0; i < height; ++i) {
            f(gr.get_cell(topleft + vec2<int>(0, i)));
            f(gr.get_cell(topleft + vec2<int>(width-1, i)));
        }
        for (int i = 0; i < width; ++i) {
            f(gr.get_cell(topleft + vec2<int>(i, 0)));
            f(gr.get_cell(topleft + vec2<int>(i, height-1)));
        }
    }
    void for_each_internal(grid<test_cell> &gr, std::function<void(test_cell&)> f) {
        for (int i = 1; i < height - 1; ++i) {
            for (int j = 1; j < width - 1; ++j) {
                f(gr.get_cell(topleft + vec2<int>(j, i)));
            }
        }
    }
};

bool rect_fits(const grid<test_cell> &gr, const rectangle &r) {
    test_cell &topleft = gr.get_cell(r.topleft);
    test_cell &bottomright = gr.get_cell(r.bottomright);
    return topleft.space_right >= r.width &&
           topleft.space_down >= r.height &&
           bottomright.space_left >= r.width &&
           bottomright.space_up >= r.height;
}

void with_rects(grid<test_cell> &gr, int width, int height,
                std::function<void(rectangle&)> f) {

    gr.for_each([&](test_cell &c) {
        rectangle r(width, height, c.coord);
        if (rect_fits(gr, r)) {
            f(r);
        }
    });
}

bool contains_rect(grid<test_cell> &gr, int width, int height) {
    for (int i = 0; i < gr.height; ++i) {
        for (int j = 0; j < gr.width; ++j) {
            rectangle r(width, height, gr[i][j].coord);
            if (rect_fits(gr, r)) {
                return true;
            }
        }
    }
    return false;
}

int find_biggest_square_size_linear(grid<test_cell> &gr) {
    for (int i = 1; i < gr.width; ++i) {
        if (!contains_rect(gr, i, i)) {
            return i - 1;
        }
    }
    return 0;
}

std::list<rectangle> find_biggest_squares_linear(grid<test_cell> &gr) {
    int size = find_biggest_square_size_linear(gr);
    std::list<rectangle> ret;
    with_rects(gr, size, size, [&](rectangle &r) {
        ret.push_back(r);
    });
    return ret;
}

void expand_rect(grid<test_cell> &gr, rectangle &r) {
    int min_space;

    min_space = gr.width;
    for (int i = 0; i < r.width; ++i) {
        min_space = std::min(min_space, gr.get_cell(r.topleft + vec2<int>(i, r.height-1)).space_down);
    }
    r.height += (min_space - 1);
    r.sync();
    min_space = gr.height;
    for (int i = 0; i < r.height; ++i) {
        min_space = std::min(min_space, gr.get_cell(r.topleft + vec2<int>(r.width-1, i)).space_right);
    }
    r.width += (min_space - 1);
    r.sync();

}

bool test_water_edge(grid<test_cell> &gr, test_cell &c, direction::direction_t a, direction::direction_t b) {
    return gr.with_neighbour<bool, false>(c, a, [](test_cell &n) {return n.is_water_hard_edge;}) &&
           gr.with_neighbour<bool, false>(c, b, [](test_cell &n) {return n.is_water_hard_edge;});
}


bool test_wall(grid<test_cell> &gr, test_cell &c, direction::direction_t a, direction::direction_t b) {
    return gr.with_neighbour<bool, false>(c, a, [](test_cell &n) {return n.is_hard_wall;}) &&
           gr.with_neighbour<bool, false>(c, b, [](test_cell &n) {return n.is_hard_wall;});
}

void find_entrance_candidates(grid<test_cell> &gr) {
    gr.for_each([&](test_cell &c) {
        if (c.is_hard_wall && gr.is_internal_cell(c)) {
            if (test_wall(gr, c, direction::east, direction::west)) {
                c.is_entrance_candidate = true;
                if (gr.with_neighbour<bool, false>(c, direction::north, [](test_cell &n) {return !n.is_enclosed;})) {
                    c.entrance_direction = direction::north;
                } else {
                    c.entrance_direction = direction::south;
                }
            } else if (test_wall(gr, c, direction::north, direction::south)) {
                c.is_entrance_candidate = true;
                if (gr.with_neighbour<bool, false>(c, direction::east, [](test_cell &n) {return !n.is_enclosed;})) {
                    c.entrance_direction = direction::east;
                } else {
                    c.entrance_direction = direction::west;
                }
            }
        }


        if (c.is_entrance_candidate) {
            c.entrance_outside_cell = gr.get_neighbour(c, c.entrance_direction);
            if (c.entrance_outside_cell->is_water && !c.entrance_outside_cell->is_bridge_candidate) {
                c.is_entrance_candidate = false;
            }
        }
/*
        if (c.is_entrance_candidate) {


            char ch = '=';
            switch (c.entrance_direction) {
            case direction::north:
                ch = '^';
                break;
            case direction::south:
                ch = 'v';
                break;
            case direction::east:
                ch = '>';
                break;
            case direction::west:
                ch = '<';
                break;
            default:
                break;
            }
            c.ch = ch;
        }
*/
    });
}

void find_edge_candidates(grid<test_cell> &gr) {
    gr.for_each([&](test_cell &c) {


        if (c.is_water_hard_edge) {
            if (test_water_edge(gr, c, direction::east, direction::west)) {
                c.is_bridge_candidate = true;
                if (gr.with_neighbour<bool, false>(c, direction::north, [](test_cell &n) {return n.is_water;})) {
                    c.bridge_direction = direction::north;
                } else {
                    c.bridge_direction = direction::south;
                }
            } else if (test_water_edge(gr, c, direction::north, direction::south)) {
                c.is_bridge_candidate = true;
                if (gr.with_neighbour<bool, false>(c, direction::east, [](test_cell &n) {return n.is_water;})) {
                    c.bridge_direction = direction::east;
                } else {
                    c.bridge_direction = direction::west;
                }
            }
        }

/*

        if (c.is_bridge_candidate) {
            char ch = '=';
            switch (c.bridge_direction) {
            case direction::north:
                ch = '^';
                break;
            case direction::south:
                ch = 'v';
                break;
            case direction::east:
                ch = '>';
                break;
            case direction::west:
                ch = '<';
                break;
            default:
                break;
            }
            c.ch = ch;
        }
*/
    });
}

bool is_connected_bridge_candidate(grid<test_cell> &gr, test_cell &cand) {
    test_cell *ptr = &cand;

    while (ptr && ptr->is_water) {
        if (ptr->is_bridge_candidate && direction::is_opposite(ptr->bridge_direction, cand.bridge_direction)) {
            return true;
        }
        ptr = gr.get_neighbour(*ptr, cand.bridge_direction);
    }
    return false;
}

void connect_bridge_candidates(grid<test_cell> &gr) {
    gr.for_each([&](test_cell &c) {
        if (c.is_bridge_candidate && is_connected_bridge_candidate(gr, c)) {
            test_cell *far_side = &c;
            while (far_side->is_water) {
//                far_side->ch = '+';
                c.bridge_far_side = far_side;
                far_side = gr.get_neighbour(*far_side, c.bridge_direction);
            }
            test_cell *near_side = gr.get_neighbour(c, direction::get_opposite(c.bridge_direction));
            near_side->group->connectable_groups[far_side->classification] = true;
            near_side->group->potential_bridges[far_side->classification].push_back(near_side);
        } else {
            c.is_bridge_candidate = false;
        }
    });
}
void reconnect_bridge_candidates(grid<test_cell> &gr) {
    gr.for_each([&](test_cell &c) {
        if (c.is_bridge_candidate && is_connected_bridge_candidate(gr, c)) {
            test_cell *far_side = &c;
            while (far_side->is_water) {
//                far_side->ch = '+';
                c.bridge_far_side = far_side;
                far_side = gr.get_neighbour(*far_side, c.bridge_direction);
            }
            test_cell *near_side = gr.get_neighbour(c, direction::get_opposite(c.bridge_direction));
            if (far_side->reclas != -1 && near_side->reclas != -1) {
                near_side->group->connectable_groups[far_side->reclas] = true;
                near_side->group->potential_bridges[far_side->reclas].push_back(&c);
            } else {
                c.is_bridge_candidate = false;
            }
        } else {
            c.is_bridge_candidate = false;
        }
    });
}
void grow_wall(grid<test_cell> &gr, rectangle &r, int amount) {
    std::list<test_cell*> queue;
    std::list<test_cell*> enclosure;
    r.for_each(gr, [&](test_cell &c) {
        c.expand_dist = 0;
        c.is_enclosed = true;
        queue.push_back(&c);
        enclosure.push_back(&c);
    });
    int current = 0;
    while (true) {
        test_cell *ptr = queue.front();
        queue.pop_front();
        current = ptr->expand_dist + 1;

        if (current > amount) {
            break;
        }

        gr.for_each_cardinal_neighbour(*ptr, [&](test_cell &nei) {
            if (!nei.is_water && !nei.is_enclosed) {
                nei.expand_dist = current;
                nei.is_enclosed = true;
                queue.push_back(&nei);
                enclosure.push_back(&nei);
            }
        });
    }

    std::for_each(enclosure.begin(), enclosure.end(), [&](test_cell *c) {
        c->ch = c->expand_dist + '0';
        c->data = COLOR_WHITE;
        int count = 0;
        gr.for_each_neighbour(*c, [&](test_cell &nei) {
            if (nei.is_enclosed) {
                ++count;
            }
        });
        c->is_wall = count != 8;

        count = 0;
        gr.for_each_cardinal_neighbour(*c, [&](test_cell &nei) {
            if (nei.is_enclosed) {
                ++count;
            }
        });
        c->is_hard_wall = count != 4;

        if (c->is_hard_wall) {
            c->ch = '#';
        } else if (c->is_wall) {
            c->ch = '#';
        } else {
            c->ch = '.';
        }
    });

}

void classify_group(grid<test_cell> &gr, cell_group &g) {
    std::for_each(g.cells.begin(), g.cells.end(), [&](test_cell *c) {
        if (!g.is_border_adjacent) {
            int count = 0;
            gr.for_each_neighbour(*c, [&](test_cell &nei) {++count;});
            if (count != 8) {
                g.is_border_adjacent = true;
            }
        }
        if (c->is_enclosed) {
            g.contains_enclosure = true;
        }
    });
}

void find_enclosure_accessable_groups(grid<test_cell> &gr, cell_group &enclosure, std::list<cell_group*> &groups) {
    std::vector<bool> seen;
    std::for_each(enclosure.cells.begin(), enclosure.cells.end(), [&](test_cell *c) {
        if (c->is_entrance_candidate) {
            test_cell *outside = c->entrance_outside_cell;
            if (outside->is_water) {
                assert(outside->is_bridge_candidate);
                assert(outside->bridge_far_side != nullptr);
                outside = gr.get_neighbour(*outside->bridge_far_side, outside->bridge_direction);

            }

            c->entrance_final_outside_cell = outside;

            cell_group &group = *(outside->group);
            while (group.classification >= seen.size()) {
                seen.push_back(false);
            }

            if (!seen[group.classification]) {
                groups.push_back(&group);
            }

            seen[group.classification] = true;

        }
    });
}

void make_bridge_from_cell(grid<test_cell> &gr, test_cell &c) {
    test_cell *tmp = &c;
    while (tmp->is_water) {
        tmp->is_bridge = true;
        tmp = gr.get_neighbour(*tmp, c.bridge_direction);
    }
}

void choose_enclosure_entrances(grid<test_cell> &gr, cell_group &enclosure, cell_group &group) {
    std::vector<test_cell*> candidates;
    std::for_each(enclosure.cells.begin(), enclosure.cells.end(), [&](test_cell *c) {
        if (c->is_entrance_candidate) {
            assert(c->entrance_outside_cell);

            if (c->entrance_final_outside_cell->group == &group) {
                candidates.push_back(c);
            }
        }
    });

    test_cell *door = candidates[arithmetic::random_int(0, candidates.size())];
    door->is_entrance = true;
    door->ch = '+';

    if (door->entrance_outside_cell->is_water) {
        assert(door->entrance_outside_cell->is_bridge_candidate);
        make_bridge_from_cell(gr, *door->entrance_outside_cell);
    }
}
void choose_enclosure_entrances(grid<test_cell> &gr, cell_group &enclosure, std::list<cell_group*> &groups) {
    std::for_each(groups.begin(), groups.end(), [&](cell_group *group) {
        choose_enclosure_entrances(gr, enclosure, *group);
    });
}


void choose_bridges(grid<test_cell> &gr, std::list<cell_group> &groups) {
//    std::for_each(groups.begin(), groups.end(), [&](cell_group &group) {
    for (auto it = groups.begin(); it != groups.end(); ++it) {
        for (unsigned int i = 0; i < it->potential_bridges.size(); ++i) {
//        std::for_each(it->potential_bridges.begin(), it->potential_bridges.end(), [&](std::list<test_cell*> &bridges) {
            std::list<test_cell*> &bridges = it->potential_bridges[i];
            if (bridges.empty()) {
                continue;
            }
            std::vector<test_cell*> bridge_vec{std::begin(bridges), std::end(bridges)};
            test_cell *c = bridge_vec[arithmetic::random_int(0, bridge_vec.size())];
            make_bridge_from_cell(gr, *c);
            test_cell *other_side = gr.get_neighbour(*(c->bridge_far_side), c->bridge_direction);
            other_side->group->potential_bridges[it->classification].clear();
//            c->ch = '0' + i;
//
//            c->bridge_far_side->group->potential_bridges.r();//[group.classification].clear();
        }       
//        });
    }
//    });
}

int main(int argc, char *argv[]) {
    long int seed = time(NULL);
//    long int seed = 1435989932;
//    long int seed = 1435465862;
//    long int seed = 1435475695;
    std::cout << seed << std::endl;
    srand(seed);
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
    

    bool has_water = false;
    while (!has_water) {
        pg.clear();
        dg.for_each([&](test_cell &c) {
            c.noise = pg.get_noise(c.centre * 0.1);
            c.data = COLOR_GREEN;
            c.ch = '.';
        });

        has_water = plot_rivers(dg);
    }

    dg.for_each([&](test_cell &c) {
        if (c.is_water_end) {
            /*
            int count = 0;
            dg.for_each_neighbour(c, [&](test_cell &nei) {
                if (nei.ch == '~') {
                    ++count;
                }
            });
            */

            if (c.ch == '~') {
                c.ch = '*';
            } else {
                c.is_water_end = false;
            }
        }
    });
    //continue_water_to_border(dg);   

    dg.for_each([&](test_cell &c) {
        if (c.ch == '~') {
            c.data = COLOR_BLUE;
        } else {
            c.ch = '.';
            c.data = COLOR_GREEN;
        }
    });

    dg.for_each([&](test_cell &c) {
        if (c.ch == '~') {
            c.data = COLOR_BLUE;
            c.generation = 1;
            c.core = &c;
        } else {
            c.data = COLOR_GREEN;
            c.generation = -1;
            c.core = nullptr;
        }
    });


    grow(dg, 0);
    grow(dg, 1);
    grow(dg, 2);
#define REST1
#ifdef REST1

    std::list<cell_group> groups;
    classify(dg, groups);

    cell_group *water_group = nullptr;
    std::for_each(groups.begin(), groups.end(), [&](cell_group &g) {
        if (g.cells.front()->ch == '~') {
            water_group = &g;
        }
    });


    std::for_each(water_group->cells.begin(), water_group->cells.end(),
        [&](test_cell *c) {
        c->is_water = true;
        c->is_open = false;
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

    for (int i = 0; i < 100; ++i) {
        tree_tick(dg);
    }

    update_chars(dg);

    compute_space(dg);

    std::list<rectangle> rects = find_biggest_squares_linear(dg);

    rectangle *rectptr = &rects.front();
    int area = rectptr->get_area();
    std::for_each(rects.begin(), rects.end(), [&](rectangle &r) {
        expand_rect(dg, r);
        int new_area = r.get_area();
        if (new_area > area) {
            area = new_area;
            rectptr = &r;
        }
    });

    rectangle &rect = *rectptr;

    grow_wall(dg, rect, 4);

    mark_water_edge(dg);

    find_edge_candidates(dg);

    connect_bridge_candidates(dg);

    cell_group *enc_group = nullptr;
    std::for_each(groups.begin(), groups.end(), [&](cell_group &g) {
        classify_group(dg, g);
        if (g.contains_enclosure) {
            enc_group = &g;
        }
    });

    classify_enclosure_group(dg, *enc_group, groups);

    find_entrance_candidates(dg);

    std::list<cell_group*> accessable_groups;
    find_enclosure_accessable_groups(dg, *enc_group, accessable_groups);

    choose_enclosure_entrances(dg, *enc_group, accessable_groups);

    std::list<cell_group> groups2;
    reclassify(dg, groups2);
    reconnect_bridge_candidates(dg);
    
    choose_bridges(dg, groups2);

    dg.for_each([](test_cell &c) {

        if (c.is_bridge) {
            c.ch = '=';
            c.data = COLOR_RED;
        }

    });

/*
    dg.for_each([](test_cell &c) {
        if (c.reclas != -1) {
            c.ch = '0' + c.reclas;
        }
    });
   
    std::for_each(groups.begin(), groups.end(), [&](cell_group &g) {
        if (!g.is_enclosure_adjacent) {
            std::for_each(g.cells.begin(), g.cells.end(), [&](test_cell *c) {
                c->ch = 'a' + g.classification;
            });
        }
    });
    std::for_each(groups.begin(), groups.end(), [&](cell_group &g) {
        if (g.is_enclosure_adjacent) {
            std::for_each(g.cells.begin(), g.cells.end(), [&](test_cell *c) {
                c->ch = 'A' + g.classification;
            });
        }
    });
*/

#endif
#ifdef DRAW
    dg.for_each([&](test_cell &c) {
        wmove(stdscr, c.y_coord, c.x_coord);
        wattron(stdscr, COLOR_PAIR(c.data));
        waddch(stdscr, c.ch);
        wattroff(stdscr, COLOR_PAIR(c.data));

    });

    wmove(stdscr, 41, 0);

    wgetch(stdscr);
    endwin();
#endif
    return 0;
}
