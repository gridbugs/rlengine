#ifndef _CONWAY_GENERATOR_HPP_
#define _CONWAY_GENERATOR_HPP_

#include "world/generator.hpp"
#include "world/grid.hpp"
#include "world/cell.hpp"
#include "world/world_cell.hpp"
#include <deque>
#include <vector>
#include <utility>

class conway_generator : public generator {

    private:
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

        void will_die() {
            alive_next_ = false;
        }
        void will_resurrect() {
            alive_next_ = true;
        }
        void step() {
            alive = alive_next_;
        }

        bool visited = false;
        int group_id = -1;
    };


    class conway_grid : public grid<conway_cell> {
        public:
        conway_grid(const int width, const int height) :
            grid<conway_generator::conway_cell>(width, height)
        {}

        void progress(const int live_min_, const int live_max_,
                      const int resurrect_min_, const int resurrect_max_) {

            for (typename conway_grid::iterator it = this->begin(); it != this->end(); ++it) {
                int alive_neighbour_count = 0;
                for (typename conway_grid::neighbour_iterator n_it = this->neighbour_begin(*it);
                    n_it != this->neighbour_end(*it);
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

                if (this->get_distance_to_edge(*it) == 0) {
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

            for (typename conway_grid::iterator it = this->begin(); it != this->end(); ++it) {
                it->step();
            }

        }



        void clean() {
            for (typename conway_grid::iterator it = this->begin(); it != this->end(); ++it) {
                int count = 0;
                for (typename conway_grid::neighbour_iterator n_it = this->neighbour_begin(*it);
                    n_it != this->neighbour_end(*it); ++n_it) {
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
        void flood() {

            int current_id = 0;
            std::vector<std::pair<bool,int>> group_sizes;
            int count;

            for (typename conway_grid::iterator it = this->begin(); it != this->end(); ++it) {
                if (it->visited) {
                    continue;
                }
                count = 0;

                std::deque<conway_cell*> q;
                q.push_front(&(*it));
                it->visited = true;
                while (!q.empty()) {
                    conway_cell *c = q.back();
                    q.pop_back();
                    ++count;
                    c->group_id = current_id;

                    for (typename conway_grid::neighbour_iterator n_it = this->cardinal_neighbour_begin(*c);
                        n_it != this->cardinal_neighbour_end(*c); ++n_it) {

                        if ((n_it->alive == c->alive) && !n_it->visited) {
                            n_it->visited = true;
                            q.push_front(&(*n_it));
                        }
                    }
                }

                group_sizes.push_back(std::pair<bool,int>(it->alive, count));
                count = 0;
                ++current_id;
            }


            int max_count = -1;
            int max_id;

            for (std::vector<std::pair<bool,int>>::iterator it = group_sizes.begin(); it != group_sizes.end(); ++it) {
                if (it->first && it->second > max_count) {
                    max_count = it->second;
                    max_id = it->first;
                }
            }


            for (typename conway_grid::iterator it = this->begin(); it != this->end(); ++it) {
                if (!it->alive && it->group_id != max_id) {
                    it->alive = true;
                }
            }

        }
    };
    int generate_attempt(conway_grid &cg) {
        for (int i = 0; i < 5; ++i) {
            cg.progress(4, 8, 5, 5);
        }

        for (int i = 0; i < 5; ++i) {
            cg.clean();
        }

        cg.flood();

        int count = 0;

        for (typename conway_grid::iterator it = cg.begin(); it != cg.end(); ++it) {
            if (!it->alive) {
                ++count;
            }
        }


        return count;
    }

    public:
    void generate(world &w) {
        grid<world_cell> &map = w.maps[0];

        for (;;) {
            conway_grid cg(w.width, w.height);

            if (generate_attempt(cg) < 1200) {
                continue;
            }


            for (typename grid<world_cell>::iterator it = map.begin(); it != map.end(); ++it) {
                conway_cell &c = cg.get_cell(it->coord);
                if (c.alive) {
                    it->set_data(new game_wall());
                } else {
                    it->set_data(new game_floor());
                }
            }

            break;
        }
    }
};

#endif
