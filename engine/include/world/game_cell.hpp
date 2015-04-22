#ifndef _GAME_CELL_HPP_
#define _GAME_CELL_HPP_

#include "world/cell.hpp"

class game_cell_data  {
    public:
    virtual bool is_opaque() = 0;
    virtual bool is_solid() = 0;

    virtual ~game_cell_data() {}
};

class game_wall : public game_cell_data {
    public:
    bool is_opaque() {return true;}
    bool is_solid() {return true;}

    ~game_wall() {}
};

class game_floor : public game_cell_data {
    public:
    bool is_opaque() {return false;}
    bool is_solid() {return false;}
    ~game_floor() {}
};

class game_cell : public cell {

    protected:
    game_cell_data *data_;

    public:
    game_cell(const int x, const int y) :
        cell(x, y), data_(nullptr)
    {

    }

    void set_data(game_cell_data *data) {
        if (data_ != nullptr) {
            delete data_;
        }
        data_ = data;
    }

    bool is_opaque() {return data_->is_opaque();}
    bool is_solid() {return data_->is_solid();}

    ~game_cell() {
        if (data_ != nullptr) {
            delete data_;
        }
    }

};

#endif
