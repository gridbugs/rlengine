#ifndef _TRANSACTION_QUEUE_
#define _TRANSACTION_QUEUE_

#include "io/curses.hpp"
#include <list>
#include <memory>

template <typename T, typename C, typename W> class transaction;
template <typename C, typename W, typename T> class world;

template <typename T, typename C, typename W> class transaction_queue {
    public:
    typedef transaction<T, C, W> transaction_t;
    typedef std::unique_ptr<transaction_t> transaction_ptr_t;
    typedef world<C, W, T> world_t;
    private:
    std::list<transaction_ptr_t> queue_;

    public:
    void register_transaction(transaction_ptr_t t) {
        queue_.push_front(std::move(t));
    }

    int process_all(world_t &w) {
        int time = 0;
        while (!queue_.empty()) {
            transaction_ptr_t &t = queue_.back();
            time += (*t)(w);
            queue_.pop_back();
        }
        return time;
    }
};

#endif
