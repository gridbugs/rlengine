#include "util/arith.hpp"

bool arithmetic::is_integer(double x) {
    return fabs(x - round(x)) == 0;
}
double arithmetic::random_double(double min, double max) {
    return (static_cast<double>(rand())/RAND_MAX) * (max - min) + min;
}
