#ifndef _ARITH_HPP_
#define _ARITH_HPP_

#include <cmath>

#define BIT(x) (1<<x)


#include <algorithm>

namespace arithmetic {
    template <typename T> T constrain(T min, T x, T max) {
        return std::min(std::max(x, min), max);
    }

    bool is_integer(double x);

    double random_double(double min, double max);
    int random_int(int min, int max);
};

#endif
