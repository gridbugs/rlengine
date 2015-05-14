#ifndef _VEC2_HPP_
#define _VEC2_HPP_

#include <iostream>
#include <array>

template <typename T=double> class vec2 {
    protected:
    std::array<T, 2> arr;

    public:
    
    static const unsigned int X_IDX = 0;
    static const unsigned int Y_IDX = 1;
    static unsigned int get_other_index(unsigned int idx) {
        return 1 - idx;
    }
    
    vec2(const T x, const T y) : arr { {x, y} }, x(arr[0]), y(arr[1]) {}
    vec2(const vec2<T> &v) : vec2(v.x, v.y) {}
    vec2() : vec2(0, 0) {}

    T &x;
    T &y;

    vec2<T> operator+(const vec2<T> &v) const {
        return vec2(x+v.x, y+v.y);
    }
    vec2<T> operator-(const vec2<T> &v) const {
        return vec2(x-v.x, y-v.y);
    }
    friend vec2<T> operator*(const vec2<T> &v, const T &s) {
        return vec2(v.x*s, v.y*s);
    }
    friend vec2<T> operator*(const T &s, const vec2<T> &v) {
        return vec2(v.x*s, v.y*s);
    }
    friend vec2<T> operator/(const vec2<T> &v, const T &s) {
        return vec2(v.x/s, v.y/s);
    }

    T dot(const vec2<T> &v) const {
        return x*v.x+y*v.y;
    }

    const T &operator[](const unsigned int idx) const {
        return arr[idx];
    }

    T &operator[](const unsigned int idx) {
        return arr[idx];
    }
 
    vec2<T>& operator=(const vec2<T>& v) {
        arr = v.arr;
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &out, const vec2<T> &v) {
        return out << "(" << v.x << ", " << v.y << ")";
    }

};

#endif
