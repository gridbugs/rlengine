#ifndef _WORLD_DIMENIONS_HPP_
#define _WORLD_DIMENIONS_HPP_

class world_dimensions {
    public:
    virtual int get_width() const = 0;
    virtual int get_height() const = 0;
    virtual int get_depth() const = 0;
};

#endif
