#ifndef _GAME_CELL_HPP_
#define _GAME_CELL_HPP_

#include "world/cell.hpp"
#include "world/game_grid.hpp"

class game_cell_data  {
    public:
    virtual bool is_opaque() const = 0;
    virtual bool is_solid() const = 0;

    virtual ~game_cell_data() {}
};

class game_wall : public game_cell_data {
    public:
    bool is_opaque() const {return true;}
    bool is_solid() const {return true;}

    ~game_wall() {}
};

class game_floor : public game_cell_data {
    public:
    bool is_opaque() const {return false;}
    bool is_solid() const {return false;}
    ~game_floor() {}
};

class game_cell : public game_cell_interface {

    protected:
    game_cell_data *data_;

    public:
    game_cell(const int x, const int y) :
        game_cell_interface(x, y), data_(nullptr)
    {

    }

    void set_data(game_cell_data *data) {
        if (data_ != nullptr) {
            delete data_;
        }
        data_ = data;
    }

    bool is_opaque() const {return data_->is_opaque();}
    bool is_solid() const {return data_->is_solid();}

    ~game_cell() {
        if (data_ != nullptr) {
            delete data_;
        }
    }

};

#endif
