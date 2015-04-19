#ifndef _NON_NULL_ITERATOR_HPP_
#define _NON_NULL_ITERATOR_HPP_



// here A is a container of Bs, and B must be a pointer
template <typename A,typename B>
class non_null_iterator {
    public:
    non_null_iterator(A &container, const int size, const int start_idx=0) : 
        container(container), size(size) 
    {
        idx = start_idx;
        increment_while_null();
    }

    B& operator*();
    B* operator->() const {
        return container[idx];
    }
    non_null_iterator<A,B>& operator++();
    non_null_iterator<A,B> operator++(int);
    non_null_iterator<A,B>& operator--();
    non_null_iterator<A,B> operator--(int);
    bool operator==(const non_null_iterator<A,B> &it) const;
    bool operator!=(const non_null_iterator<A,B> &it) const;

    private:
    A& container;
    int idx;
    const int size;
    void increment_while_null() {
        while(idx<size && container[idx]==nullptr) {
            ++idx;
        }
    }
    void decrement_while_null() {
        while(idx>=0 && container[idx]==nullptr) {
            --idx;
        }
    }
};

template <typename A, typename B> non_null_iterator<A,B>& non_null_iterator<A,B>::operator++() {
    ++idx;
    increment_while_null();
    return *this;
}
template <typename A, typename B> non_null_iterator<A,B> non_null_iterator<A,B>::operator++(int) {
    non_null_iterator<A,B> ret = *this;
    ++idx;
    increment_while_null();
    return ret;
}
template <typename A, typename B> non_null_iterator<A,B>& non_null_iterator<A,B>::operator--() {
    --idx;
    decrement_while_null();
    return *this;
}
template <typename A, typename B> non_null_iterator<A,B> non_null_iterator<A,B>::operator--(int) {
    non_null_iterator<A,B> ret = *this;
    --idx;
    decrement_while_null();
    return ret;
}

template <typename A, typename B> B& non_null_iterator<A,B>::operator*() {
    return &container[idx];
}

template <typename A, typename B> bool non_null_iterator<A,B>::operator==(const non_null_iterator<A,B> &it) const {
    return idx == it.idx;
}
template <typename A, typename B> bool non_null_iterator<A,B>::operator!=(const non_null_iterator<A,B> &it) const {
    return idx != it.idx;
}

#endif
