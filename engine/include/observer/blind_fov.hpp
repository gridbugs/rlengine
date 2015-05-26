#ifndef _BLIND_FOV_HPP_
#define _BLIND_FOV_HPP_

#include "observer/observer.hpp"

template <typename C, typename W, typename K> class blind_fov : 
    public observer <C, W, K> {

    public:
    void observe(const C &c, const grid<W> &w_grid, grid<K> &k_grid) {}
};

#endif
