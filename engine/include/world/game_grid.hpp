#ifndef _GAME_GRID_HPP_
#define _GAME_GRID_HPP_

#include "world/grid.hpp"
#include "world/game_cell.hpp"

class game_grid_interface {
    public:
    virtual game_cell_interface& get_game_cell(const int x_coord, const int y_coord) const = 0;
    game_cell_interface& get_game_cell(const vec2<int> &v) const {
        return get_game_cell(v.x, v.y);
    }

    virtual int get_width() const = 0;
    virtual int get_height() const = 0;

    virtual void for_each(const std::function<void(game_cell_interface&)> &f) = 0;
    virtual void for_each(const std::function<void(const game_cell_interface&)> &f) const  = 0;
};

template <typename T> class game_grid : public grid<T>, public game_grid_interface {
    public:
    game_grid(const int width, const int height) :
        grid<T>(width, height)
    {}

    game_cell_interface& get_game_cell(const int x_coord, const int y_coord) const {
        return this->get_cell(x_coord, y_coord);
    }

    int get_width() const {return this->width;}
    int get_height() const {return this->height;}

    void for_each(const std::function<void(game_cell_interface&)> &f) {
        std::for_each(this->begin(), this->end(), f);
    }
    
    void for_each(const std::function<void(const game_cell_interface&)> &f) const {
        std::for_each(this->cells_.cbegin(), this->cells_.cend(), f);
    }
};

#endif
