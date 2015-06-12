#ifndef _CANCELLABLE_HPP_
#define _CANCELLABLE_HPP_

#include <memory>
#include <forward_list>

template <typename T> class cancellable_watcher_list :
    public std::forward_list<T*> {

    public:
    void remove_cancelled() {
        this->remove_if([](T* x) {
            return x->is_cancelled();
        });
    }
};

template <typename T> class cancellable_owner_list {
    protected:
    std::forward_list<std::unique_ptr<T>> list_;

    typedef cancellable_watcher_list<T> watcher_t;
    std::forward_list<watcher_t*> watchers_;

    public:
    void add_watcher(cancellable_watcher_list<T> &w) {
        watchers_.push_front(&w);
        std::for_each(list_.begin(), list_.end(), [&](std::unique_ptr<T> &x) {
            w.push_front(x.get());
        });
    }

    void watchers_remove_cancelled() {
        std::for_each(watchers_.begin(), watchers_.end(), [&](watcher_t *w) {
            w->remove_cancelled();
        });
    }

    /* This is marked unsafe because if any elements of list_ are cancelled
     * between the last call to watchers_remove_cancelled and this function,
     * this may leave dangling pointers in watchers.
     */
    void unsafe_remove_cancelled() {
        list_.remove_if([](std::unique_ptr<T> &x) {
            return x->is_cancelled();
        });
    }

    void remove_cancelled() {
        watchers_remove_cancelled();
        unsafe_remove_cancelled();
    }
};


#endif
