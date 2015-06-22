#ifndef _PERLIN_HPP_
#define _PERLIN_HPP_

#include "geometry/vec2.hpp"
#include <vector>
#include <array>

class perlin_gradient {
    public:
    static vec2<double> get_random_unit_vector();

    const vec2<double> unit_vector;
    perlin_gradient() :
        unit_vector(get_random_unit_vector())
    {}
};

template <typename T> class signed_vector {

    private:
    std::vector<T> positive;
    std::vector<T> negative;
    T zero;

    void resize_vector(std::vector<T> &vector, unsigned int i) {
        while (vector.size() < i) {
            vector.emplace_back();
        }
    }

    public:
    void resize_to(int i) {
        if (i >= min_index() && i <= max_index()) {
            return;
        }

        if (i > 0) {
            resize_vector(positive, i);
        } else if (i < 0) {
            resize_vector(negative, -i);
        }
    }

    int min_index() const {return -negative.size();}
    int max_index() const {return positive.size();}

    T &operator[](int i) {
        if (i > 0) {
            return positive[i-1];
        }
        if (i < 0) {
            return negative[-i-1];
        }
        return zero;
    }
};

class perlin_grid {

    private:

    /* Vector of rows.
     * Each row is a vector of gradients.
     */
    signed_vector<signed_vector<perlin_gradient>> gradient_grid;

    void resize_grid(const vec2<double> &p);
    const perlin_gradient &get_gradient(const vec2<double> &p);

    public:

    double get_noise(const vec2<double> &p);
};

#endif
