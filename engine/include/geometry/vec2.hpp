#ifndef _VEC2_HPP_
#define _VEC2_HPP_

#include <iostream>
#include <array>

template <typename T=double> class Vec2 {
    public:
    Vec2(const T x, const T y) : arr { {x, y} }, x(arr[0]), y(arr[1]) {}
    Vec2(const Vec2<T> &v) : Vec2(v.x, v.y) {}
    Vec2() : Vec2(0, 0) {}


    std::array<T, 2> arr;
    T &x;
    T &y;

    Vec2<T> operator+(const Vec2<T> &v) const {
        return Vec2(x+v.x, y+v.y);
    }
    Vec2<T> operator-(const Vec2<T> &v) const {
        return Vec2(x-v.x, y-v.y);
    }
    friend Vec2<T> operator*(const Vec2<T> &v, const T &s) {
        return Vec2(v.x*s, v.y*s);
    }
    friend Vec2<T> operator*(const T &s, const Vec2<T> &v) {
        return Vec2(v.x*s, v.y*s);
    }
    friend Vec2<T> operator/(const Vec2<T> &v, const T &s) {
        return Vec2(v.x/s, v.y/s);
    }

    T dot(const Vec2<T> &v) const {
        return x*v.x+y*v.y;
    }

    const T &operator[](const unsigned int idx) const {
        return arr[idx];
    }

    T &operator[](const unsigned int idx) {
        return arr[idx];
    }
    
    friend std::ostream &operator<<(std::ostream &out, const Vec2<T> &v) {
        return out << "(" << v.x << ", " << v.y << ")";
    }

    static const unsigned int X_IDX = 0;
    static const unsigned int Y_IDX = 1;
};

#endif
