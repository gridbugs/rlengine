#ifndef _GRID_HPP_
#define _GRID_HPP_

#include "geometry/direction.hpp"
#include "geometry/vec2.hpp"
#include "util/non_null_iterator.hpp"
#include "util/dereference_iterator.hpp"
#include <vector>
#include <array>
#include <algorithm>
#include <functional>
#include "world/cell.hpp"

class grid_interface {
    public:
    virtual void for_each_cell(const std::function<void(cell&)> &f) = 0;
    virtual void for_each_cell(const std::function<void(const cell&)> &f) const = 0;
};

template <typename T> class simple_grid : public grid_interface {

    protected:
    const int n_cells_;
    std::vector<T> cells_;
    std::vector<T*> rows_;

    public:
    typedef typename std::vector<T>::iterator iterator;
    const int width;
    const int height;

    simple_grid(const int width, const int height) :
        n_cells_(width * height),
        width(width), height(height) 
    {
        /* Create cells */
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                cells_.push_back(T(j, i));
            }
        }
 
        /* Populate pointer array */
        for (int i = 0; i < height; ++i) {
            rows_.push_back(&cells_[i*width]);
        }
    }


    T * const operator[](const int idx) const {
        return rows_[idx];
    }

    T& get_cell(const int x_coord, const int y_coord) const {
        return (*this)[y_coord][x_coord];
    }

    T& get_cell(const vec2<int> &v) const {
        return (*this)[v.y][v.x];
    }

    iterator begin() {
        return cells_.begin();
    }
    iterator end() {
        return cells_.end();
    }

    void for_each(const std::function<void(T&)> &f) {
        std::for_each(begin(), end(), f);
    }
    
    void for_each(const std::function<void(const T&)> &f) const {
        std::for_each(cells_.cbegin(), cells_.cend(), f);
    }
    
    void for_each_cell(const std::function<void(cell&)> &f) {
        for_each(f);
    }

    void for_each_cell(const std::function<void(const cell&)> &f) const {
        for_each(f);
    }
};

template <typename T> class grid : public simple_grid<T> {

    private:
    class cell_internal {
        public:
        T& cell;
        std::array<T*, direction::n_directions> neighbours;

        cell_internal(T &cell) :
            cell(cell) 
        {}


        typedef non_null_iterator<std::array<T*, direction::n_directions>, T> neighbour_iterator;

        neighbour_iterator neighbour_begin() {
            return neighbour_iterator(neighbours, direction::n_directions);
        }
        neighbour_iterator neighbour_end() {
            return neighbour_iterator(neighbours, direction::n_directions, direction::n_directions);
        }
        neighbour_iterator cardinal_neighbour_begin() {
            return neighbour_iterator(neighbours, direction::n_cardinal_directions);
        }
        neighbour_iterator cardinal_neighbour_end() {
            return neighbour_iterator(neighbours, direction::n_cardinal_directions, direction::n_cardinal_directions);
        }


        void link_neighbours(grid<T> &g) {

            for (int i = 0;i<direction::n_directions;i++) {
                neighbours[i] = nullptr;
            }

            if (cell.x_coord > 0) {
                neighbours[direction::west] = &g[cell.y_coord][cell.x_coord-1];
                if (cell.y_coord > 0) {
                    neighbours[direction::northwest] = &g[cell.y_coord-1][cell.x_coord-1];
                }
                if (cell.y_coord < g.height-1) {
                    neighbours[direction::southwest] = &g[cell.y_coord+1][cell.x_coord-1];
                }
            }
            if (cell.x_coord < g.width-1) {
                neighbours[direction::east] = &g[cell.y_coord][cell.x_coord+1];
                if (cell.y_coord > 0) {
                    neighbours[direction::northeast] = &g[cell.y_coord-1][cell.x_coord+1];
                }
                if (cell.y_coord < g.height-1) {
                    neighbours[direction::southeast] = &g[cell.y_coord+1][cell.x_coord+1];
                }
            }
            if (cell.y_coord > 0) {
                neighbours[direction::north] = &g[cell.y_coord-1][cell.x_coord];
            }
            if (cell.y_coord < g.height-1) {
                neighbours[direction::south] = &g[cell.y_coord+1][cell.x_coord];
            }

        }
    };

    std::vector<cell_internal> cell_internals_;
    std::vector<T*> border_;

    cell_internal &get_cell_internal(const int x_coord, 
                                     const int y_coord) {
        return cell_internals_[x_coord + y_coord * this->width];
    }

    typedef typename std::vector<cell_internal>::iterator iterator_internal;
    
    public:
   
    typedef typename simple_grid<T>::iterator iterator;

    grid(const int width, const int height) :
        simple_grid<T>(width, height)
    {
        
        for (iterator it = this->cells_.begin(); it != this->cells_.end(); ++it) {
            cell_internals_.push_back(*it);
        }

        /* Link neighbours */
        for (iterator_internal it = cell_internals_.begin();
            it != cell_internals_.end();
            ++it)
        {
            it->link_neighbours(*this);    
        }

        /* Compute border */
        /* top */
        for (int i = 0; i < width; ++i) {
            border_.push_back(&((*this)[0][i]));
        }
        /* right */
        for (int i = 1; i < height - 1; ++i) {
            border_.push_back(&(*this)[i][width-1]);
        }

        /* bottom */
        for (int i = width - 1; i >= 0; --i) {
            border_.push_back(&(*this)[height-1][i]);
        }

        /* left */
        for (int i = height - 2; i >= 1; --i) {
            border_.push_back(&(*this)[i][0]);
        }

    }

    const std::vector<T* const> & get_border() {return border_;}

    typedef dereference_iterator<typename std::vector<T*>::iterator, T> border_iterator;
    border_iterator border_begin() {
        border_.begin();
        return border_iterator(border_.begin());
    }
    border_iterator border_end() {
        return border_iterator(border_.end());
    }

    typedef typename cell_internal::neighbour_iterator neighbour_iterator;
    neighbour_iterator neighbour_begin(const T &cell) {
        return get_cell_internal(cell.x_coord, cell.y_coord).neighbour_begin();
    }
    neighbour_iterator neighbour_end(const T &cell) {
        return get_cell_internal(cell.y_coord, cell.y_coord).neighbour_end();
    }
    neighbour_iterator cardinal_neighbour_begin(const T &cell) {
        return get_cell_internal(cell.x_coord, cell.y_coord).cardinal_neighbour_begin();
    }
    neighbour_iterator cardinal_neighbour_end(const T &cell) {
        return get_cell_internal(cell.x_coord, cell.y_coord).cardinal_neighbour_end();
    }

    int get_distance_to_edge(T &cell) {
        return std::min(
            std::min(cell.x_coord, this->width - cell.x_coord - 1),
            std::min(cell.y_coord, this->height - cell.y_coord - 1)
        );
    }

    T* get_neighbour(T &cell, direction::direction_t direction) {
        return get_cell_internal(cell.x_coord, cell.y_coord).neighbours[direction];
    }
    
    void for_each_neighbour(const T& cell, const std::function<void(T&)> &f) {
        std::for_each(neighbour_begin(cell), neighbour_end(cell), f);
    }
    void for_each_cardinal_neighbour(const T& cell, const std::function<void(T&)> &f) {
        std::for_each(cardinal_neighbour_begin(cell), cardinal_neighbour_end(cell), f);
    }
    void for_each_border(const std::function<void(T&)> &f) {
        std::for_each(border_begin(), border_end(), f);
    }
};

template <typename T>
std::ostream& operator<<(std::ostream &out, const grid<T> &g) {
    for (int i = 0;i<g.height;i++) {
        for (int j = 0;j<g.width;j++) {
            out << g[i][j];
        }
        if (i < g.height - 1) {
            out << std::endl;
        }
    }
    return out;
}



#endif
