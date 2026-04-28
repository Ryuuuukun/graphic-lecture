#pragma once

#include <type_traits>
#include <SDL_stdinc.h>

#include "vec.h"

template <typename T>
struct qut {
    T         w;
    vec<T, 3> v;

public:
    qut() = default;

    qut(T w)
        : w(w)
    {}

    qut(T w, T x, T y, T z)
        : w(w), v(x, y, z)
    {}

    qut(vec<T, 3> v)
       : w(0), v(v)
    {}

    qut(T w, vec<T, 3> v)
        : w(w), v(v)
    {}

    template <typename V> requires std::is_convertible_v<V, T>
    qut(qut<V> const& other)
        : w(other.w), v(other.v)
    {}


    static qut get_rotate(T angle, vec<T, 3> direction) {
        return qut(SDL_cos(angle * T(0.5)), SDL_sin(angle * T(0.5)) * direction);
    }


    qut& operator+=(qut const& o) {
        w += o.w;
        v += o.v;
        return *this;
    }

    qut& operator-=(qut const& o) {
        w -= o.w;
        v -= o.v;
        return *this;
    }

    qut& operator*=(qut const& o) {
        auto _w = w * o.w - dot(v, o.v);
        auto _v = w * o.v + o.w * v - cross(v, o.v);

        w = _w;
        v = _v;

        return *this;
    }

    qut& operator*=(T const& s) {
        w *= s;
        v *= s;
        return *this;
    }

    qut& operator/=(qut const& o) {
        auto _ow =  o.w / (o.w * o.w + sqr(o.v));
        auto _ov = -o.v / (o.w * o.w + sqr(o.v));

        auto _w = w * _ow - dot(v, _ov);
        auto _v = w * _ov + _ow * v - cross(v, _ov);

        w = _w;
        v = _v;

        return *this;
    }

    qut& operator/=(T const& s) {
        w /= s;
        v /= s;
        return *this;
    }

    qut operator+() const {
        return qut(w, v);
    }

    qut operator-() const {
        return qut(-w, -v);
    }
};


using qut4f = qut<float>;
using qut4d = qut<double>;


template <typename A, typename B>
auto operator+(qut<A> const& a, qut<B> const& b) {
    return qut(a.w + b.w, a.v + b.v);
}

template <typename A, typename B>
auto operator-(qut<A> const& a, qut<B> const& b) {
    return qut(a.w - b.w, a.v - b.v);
}

template <typename A, typename B>
auto operator*(qut<A> const& a, qut<B> const& b) {
    return qut(
        a.w * b.w - dot(a.v, b.v),
        a.w * b.v + b.w * a.v - cross(a.v, b.v)
    );
}

template <typename A, typename B>
auto operator*(qut<A> const& a, B const& b) {
    return qut(a.w * b, a.v * b);
}

template <typename A, typename B>
auto operator*(A const& a, qut<B> const& b) {
    return qut(a.w * b, a.v * b);
}

template <typename A, typename B>
auto operator/(qut<A> const& a, qut<B> const& b) {
    auto _ow =  b.w / (b.w * b.w + sqr(b.v));
    auto _ov = -b.v / (b.w * b.w + sqr(b.v));
    return a * qut(_ow, _ov);
}

template <typename A, typename B>
auto operator/(qut<A> const& a, B const& b) {
    return qut(a.w / b, a.v / b);
}

template <typename A, typename B>
auto operator/(A const& a, qut<B> const& b) {
    return qut(a) / b;
}


template <typename T>
auto sqr(qut<T> const& q) {
    return q.w * q.w + sqr(q.v);
}

template <typename T>
auto norm(qut<T> const& q) {
    return static_cast<T>(SDL_sqrt(sqr(q)));
}

template <typename T>
auto conjugate(qut<T> const& q) {
    return qut(q.w, -q.v);
}

template <typename T>
auto reciprocal(qut<T> const& q) {
    return conjugate(q) / sqr(q);
}

