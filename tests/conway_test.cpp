#include "world/grid.hpp"
#include "world/cell.hpp"
#include <iostream>

#include "world/game_cell.hpp"
#include "io/curses.hpp"
#include "drawing/curses_drawer.hpp"

class conway_cell : public cell {
    private:
    bool alive_next_;
    public:
    bool alive;
    conway_cell(const int x_coord, const int y_coord) : 
        cell(x_coord, y_coord)
    {
        alive = rand()%2;
        alive_next_ = alive;
    }

    void will_die();
    void will_resurrect();
    void step();
};

void conway_cell::will_die() {
    alive_next_ = false;
}

void conway_cell::will_resurrect() {
    alive_next_ = true;
}

void conway_cell::step() {
    alive = alive_next_;
}


std::ostream& operator<<(std::ostream &out, const conway_cell &c) {
    if (c.alive) {
        return out << '#';
    } else {
        return out << ' ';
    }
}


class conway_grid : public grid<conway_cell> {
    public:
    conway_grid(const int width, const int height) :
        grid<conway_cell>(width, height)
    {}

    void progress(const int live_min, const int live_max,
                  const int resurrect_min, const int resurrect_max);
    void clean();
};



void conway_grid::progress(const int live_min_, const int live_max_,
                  const int resurrect_min_, const int resurrect_max_) {

    for (iterator it = begin(); it != end(); ++it) {
        int alive_neighbour_count = 0;
        for (neighbour_iterator n_it = neighbour_begin(*it);
            n_it != neighbour_end(*it);
            ++n_it)
        {
            if (n_it->alive) {
                ++alive_neighbour_count;
            }
        }

        int live_min = live_min_;
        int live_max = live_max_;
        int resurrect_min = resurrect_min_;
        int resurrect_max = resurrect_max_;
        
        if (get_distance_to_edge(*it) == 0) {
//            std::cout << it->x_coord << ", " << it->y_coord << std::endl;
            live_min = 0;
            live_max = 8;
            resurrect_min = 0;
            resurrect_max = 8;
            
        }

        if (it->alive) {
            if (alive_neighbour_count < live_min ||
                alive_neighbour_count > live_max) 
            {
                it->will_die();
            }
        } else {
            if (alive_neighbour_count >= resurrect_min &&
                alive_neighbour_count <= resurrect_max)
            {
                it->will_resurrect();
            }
        }
    }

    for (iterator it = begin(); it != end(); ++it) {
        it->step();
    }
}



class conway_cell_wrapper : public game_cell_data {
    
    private:
    const conway_cell &c;

    public:
    conway_cell_wrapper(const conway_cell &c) : c(c) {}

    bool is_opaque() {return c.alive;}
    bool is_solid() {return c.alive;}

    ~conway_cell_wrapper() {}
};



void conway_grid::clean() {
    for (iterator it = begin(); it != end(); ++it) {
        int count = 0;
        for (neighbour_iterator n_it = neighbour_begin(*it);
            n_it != neighbour_end(*it); ++n_it) {
            if (n_it->alive) {
                ++count;
            }

        }

        if (count > 5) {
            it->alive = true;
        }

        if (count < 2) {
            it->alive = false;
        }
    }
}

int main(int argc, char *argv[]) {
    curses::simple_start();
    srand(time(NULL));

    conway_grid g(100, 40);
    grid<game_cell> game_grid(100, 40);

    for (int i = 0; i < game_grid.height; ++i) {
        for (int j = 0; j < game_grid.width; ++j) {
            game_grid[i][j].set_data(new conway_cell_wrapper(g[i][j]));
        }
    }
    
//    for (int i = 0; i < 5; ++i) {
//        g.progress(5, 8, 6, 6);
//    }

    curses_drawer dr;

    std::string str;
    while(true) {
        dr.draw_grid(game_grid);
        g.progress(4, 8, 5, 5);
        getch();
//        g.clean();
    }


    curses::simple_stop();
    return 0;
}
