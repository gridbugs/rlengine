#ifndef _BOOLEAN_MONAD_HPP_
#define _BOOLEAN_MONAD_HPP_

#include <functional>

/* Emulates short-circuit evaluation for arbitrary bool-like
 * types in a monadic fashion. Allows a custom class implementing
 * operator&&, operator|| and operator bool to be used in place
 * of regular bool. These can track side effects in addition to
 * behaving like booleans.
 */
template <typename C> class boolean_monad {
    protected:
    C context;
    public:
    boolean_monad(C context) :
        context(context)
    {}

    boolean_monad<C> &operator&&(const std::function<C()> f) {
        if (context) {
            context = context && f();
        }
        return *this;
    }

    boolean_monad<C> &operator||(const std::function<C()> f) {
        if (!context) {
            context = context || f();
        }
        return *this;
    }

    friend C get_context(const boolean_monad<C> &bm) {
        return bm.context;
    }
};

template <typename C> class boolean_monad_non_scalar {
    C context;

    public:
    boolean_monad_non_scalar(C context) :
        context(context)
    {}
    
    boolean_monad_non_scalar<C> &operator&&(const std::function<C()> f) {
        if (context) {
            context = context && f();
        }
        return *this;
    }
    boolean_monad_non_scalar<C> &operator||(const std::function<C()> f) {
        if (!context) {
            context = context || f();
        }
        return *this;
    }
    operator C() {
        return this->context;
    }
};

#endif
