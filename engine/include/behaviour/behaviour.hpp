#ifndef _BEHAVIOUR_HPP_
#define _BEHAVIOUR_HPP_

#include "world/world.hpp"
#include "util/boolean_monad.hpp"
#include <utility>

/* Encapsulates the success or failure of an operation,
 * as well as how long the operation took. Intended for
 * use with boolean_monad.
 */
class timed_result {
    public:
    bool success;
    int time;
    timed_result(bool success, int time) :
        success(success),
        time(time)
    {}
    timed_result(bool success) :
        success(success),
        time(0)
    {}
    timed_result(std::pair<bool, int> p) :
        success(p.first),
        time(p.second)
    {}
    timed_result(std::pair<int, bool> p) :
        success(p.second),
        time(p.first)
    {}

    operator bool() const {
        return success;
    }
    timed_result operator&&(timed_result tr) const {
        return timed_result(success && tr.success, time + tr.time);
    }
    timed_result operator||(timed_result tr) const {
        return timed_result(success || tr.success, time + tr.time);
    }
};

namespace behaviour_tree {
    boolean_monad<timed_result> seq() {
        return boolean_monad<timed_result>(timed_result(true, 0));
    }
    boolean_monad<timed_result> sel() {
        return boolean_monad<timed_result>(timed_result(false, 0));
    }
};

#endif
