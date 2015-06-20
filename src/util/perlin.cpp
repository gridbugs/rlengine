#include "util/perlin.hpp"

#include <math.h>
#include "util/arith.hpp"


void perlin_grid::resize_grid(const vec2<double> &p) {
    gradient_grid.resize_to(p.y);
    gradient_grid[p.y].resize_to(p.x);
}


const perlin_gradient &perlin_grid::get_gradient(const vec2<double> &p) {
    return gradient_grid[p.y][p.x];
}

/* Function which exagerates the proximity of its inputs to 0 and 1 */
double ease_curve(double x) {
    return 6*std::pow(x, 5) - 15*std::pow(x, 4) + 10*std::pow(x, 3);
}

double perlin_grid::get_noise(const vec2<double> &p) {

    /* +----> x
     * | 
     * |    a +--------+ b
     * |      |        |
     * v      |        |
     *        |   p    |
     * y      |        |
     *      c +--------+ d
     */

    /* Find the integer coordinates surrounding p */
    const vec2<double> ca(std::floor(p.x), std::floor(p.y));
    const vec2<double> cb(std::ceil(p.x), std::floor(p.y));
    const vec2<double> cc(std::floor(p.x), std::ceil(p.y));
    const vec2<double> cd(std::ceil(p.x), std::ceil(p.y));

    /* Make sure the gradient grid is big enough for p */
    resize_grid(ca);
    resize_grid(cb);
    resize_grid(cc);
    resize_grid(cd);

    /* Look up the gradients at each integer coordinate */
    const vec2<double> &ga = get_gradient(ca).unit_vector;
    const vec2<double> &gb = get_gradient(cb).unit_vector;
    const vec2<double> &gc = get_gradient(cc).unit_vector;
    const vec2<double> &gd = get_gradient(cd).unit_vector;

    /* Find the vectors from each integer cooordinate to p */
    const vec2<double> fa = p - ca;
    const vec2<double> fb = p - cb;
    const vec2<double> fc = p - cc;
    const vec2<double> fd = p - cd;

    /* Find dot products of gradients and vectors from corners to p */
    const double ia = ga.dot(fa);
    const double ib = gb.dot(fb);
    const double ic = gc.dot(fc);
    const double id = gd.dot(fd);

    /* Find weighted averages of dot products at a and b, then for c and d */
    const double weight_x = ease_curve(p.x - ca.x);
    const double weight_y = ease_curve(p.y - ca.y);
    const double ab_avg = ia + weight_x * (ib - ia);
    const double cd_avg = ic + weight_x * (id - ic);
    const double avg = ab_avg + weight_y * (cd_avg - ab_avg);

    return avg;
}

vec2<double> perlin_gradient::get_random_unit_vector() {
    double angle = arithmetic::random_double(0, M_PI * 2);
    return vec2<>::from_radial(angle, 1);
}
