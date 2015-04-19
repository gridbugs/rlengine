#ifndef _DEREFERENCE_ITERATOR_HPP_
#define _DEREFERENCE_ITERATOR_HPP_

template <typename I, typename T>
class dereference_iterator {

    private:
    I it;

    public:
    dereference_iterator(const I &it) : 
        it(it)
        {}

    T& operator*() {
        return **it;
    }
    T* operator->() const {
        return *it;
    }
    
    dereference_iterator<I,T>& operator++() {++it;return *this;}
    dereference_iterator<I,T> operator++(int) {
        dereference_iterator<I,T> ret = *this;
        it++;
        return ret;
    }
    dereference_iterator<I,T>& operator--() {--it;return *this;}
    dereference_iterator<I,T> operator--(int) {
        dereference_iterator<I,T> ret = *this;
        it--;
        return ret;
    }

    
    bool operator==(const dereference_iterator<I,T> &it) const {
        return this->it == it.it;
    }
    bool operator!=(const dereference_iterator<I,T> &it) const {
        return this->it != it.it;
    }

};

#endif
