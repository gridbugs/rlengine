#ifndef _OBSERVER_HPP_
#define _OBSERVER_HPP_

#include "world/grid.hpp"

template <typename C, typename W, typename K> class observer {
    public:
    virtual void observe(const C &c, const grid<W> &w_grid, grid<K> &k_grid) = 0;
};

#endif
