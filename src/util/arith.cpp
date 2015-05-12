#include "util/arith.hpp"
    
bool arithmetic::is_integer(double x) {
    return fabs(x - round(x)) == 0;
}
