#ifndef _TRANSACTION_QUEUE_
#define _TRANSACTION_QUEUE_

#include "io/curses.hpp"
#include "transaction/transaction.hpp"

#include <list>
#include <memory>

class world;

class transaction_queue {
    public:
    typedef std::unique_ptr<transaction> transaction_ptr_t;
    private:
    std::list<transaction_ptr_t> queue_;

    public:
    void register_transaction(transaction_ptr_t t) {
        queue_.push_front(std::move(t));
    }

    int process_all(world &w) {
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
