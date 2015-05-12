#ifndef _ARITH_HPP_
#define _ARITH_HPP_

#define BIT(x) (1<<x)


#include <algorithm>

namespace arithmetic {
    template <typename T> T constrain(T min, T x, T max) {
        return std::min(std::max(x, min), max);
    }
};

#endif
