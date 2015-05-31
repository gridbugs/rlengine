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
    timed_result() : success(false), time(0) {}

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

class timed_result_monad : public boolean_monad_non_scalar<timed_result> {
    public:
    timed_result_monad(timed_result t) :
        boolean_monad_non_scalar<timed_result>(t)
    {}
};

namespace behaviour_tree {
    timed_result_monad seq() {
        return timed_result_monad(timed_result(true, 0));
    }
    timed_result_monad sel() {
        return timed_result_monad(timed_result(false, 0));
    }

    template <typename A, typename B>
    std::function<timed_result()> with(A t, B f) {
        return std::bind(f, t);
    }
};

#endif
