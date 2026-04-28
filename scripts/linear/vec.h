#pragma once

#include <SDL_stdinc.h>
#include <type_traits>
#include <cmath>

struct vec_tag {

};

template <typename T>
concept is_vector = std::derived_from<std::remove_cvref_t<T>, vec_tag>;

template <typename T, size_t DIM>
struct vec;

template <typename T>
struct vec<T, 2> : vec_tag {
    using value_type = T;
    static constexpr size_t DIM = 2;

    T x;
    T y;

public:
    vec() = default;

    vec(T v)
        : x(v), y(v)
    {}

    vec(T x, T y)
        : x(x), y(y)
    {}

    template <typename V> requires std::is_convertible_v<V, T>
    vec(vec<V, 2> const& other)
        : x(other.x), y(other.y)
    {}

    T* data() {
        return &x;
    }

    T const* data() const {
        return &x;
    }

    T& operator[](size_t i) {
        return data()[i];
    }

    T const& operator[](size_t i) const {
        return data()[i];
    }
};


using vec2f = vec<float, 2>;
using vec2d = vec<float, 2>;


template <typename A, typename B>
auto operator+(vec<A, 2> const& a, vec<B, 2> const& b) -> vec<decltype(A{ } + B{ }), 2> {
    return { a.x + b.x, a.y + b.y };
}

template <typename A, typename B>
auto operator-(vec<A, 2> const& a, vec<B, 2> const& b) -> vec<decltype(A{ } - B{ }), 2> {
    return { a.x - b.x, a.y - b.y };
}

template <typename A, typename B>
auto operator*(vec<A, 2> const& a, B const& b) -> vec<decltype(A{ } * B{ }), 2> {
    return { a.x * b, a.y * b };
}

template <typename A, typename B>
auto operator*(A const& a, vec<B, 2> const& b) -> vec<decltype(A{ } * B{ }), 2> {
    return { a * b.x, a * b.y };
}

template <typename A, typename B>
auto operator/(vec<A, 2> const& a, B const& b) -> vec<decltype(A{ } / B{ }), 2> {
    return { a.x / b, a.y / b };
}

template <typename A, typename B>
auto dot(vec<A, 2> const& a, vec<B, 2> const& b) -> decltype(A{ } * B{ }) {
    return a.x * b.x + a.y + b.y;
}

template <typename A, typename B>
auto cross(vec<A, 2> const& a, vec<B, 2> const& b) -> vec<decltype(A{ } * B{ }), 3> {
    return { 0, 0, a.x * b.y - a.y * b.x };
}

template <typename T>
vec<T, 2> max(vec<T, 2> const& a, vec<T, 2> const& b) {
    return {
        std::max(a.x, b.x),
        std::max(a.y, b.y)
    };
}

template <typename T>
vec<T, 2> min(vec<T, 2> const& a, vec<T, 2> const& b) {
    return {
        std::min(a.x, b.x),
        std::min(a.y, b.y)
    };
}


template <typename T>
struct vec<T, 3> : vec_tag {
    using value_type = T;
    static constexpr size_t DIM = 3;

    T x;
    T y;
    T z;

public:
    vec() = default;

    vec(T v)
        : x(v), y(v), z(v)
    {}

    vec(T x, T y, T z)
        : x(x), y(y), z(z)
    {}

    template <typename V> requires std::is_convertible_v<V, T>
    vec(vec<V, 3> const& other)
        : x(other.x), y(other.y), z(other.z)
    {}

    T* data() {
        return &x;
    }

    T const* data() const {
        return &x;
    }

    T& operator[](size_t i) {
        return data()[i];
    }

    T const& operator[](size_t i) const {
        return data()[i];
    }

    vec& operator+=(vec const& o) {
        x += o.x; y += o.y; z += o.z;
        return *this;
    }

    vec& operator-=(vec const& o) {
        x -= o.x; y -= o.y; z -= o.z;
        return *this;
    }

    vec& operator*=(T const& s) {
        x *= s; y *= s; z *= s;
        return *this;
    }

    vec& operator/=(T const& s) {
        x /= s; y /= s; z /= s;
        return *this;
    }

    vec operator+() const {
        return vec(x, y, z);
    }

    vec operator-() const {
        return vec(-x, -y, -z);
    }
};


using vec3f = vec<float, 3>;
using vec3d = vec<double, 3>;


template <typename A, typename B>
auto operator+(vec<A, 3> const& a, vec<B, 3> const& b) -> vec<decltype(A{ } + B{ }), 3> {
    return { a.x + b.x, a.y + b.y, a.z + b.z };
}

template <typename A, typename B>
auto operator+(vec<A, 3> const& a, B const& b) -> vec<decltype(A{ } + B{ }), 3> {
    return { a.x + b, a.y + b, a.z + b };
}

template <typename A, typename B>
auto operator+(A const& a, vec<B, 3> const& b) -> vec<decltype(A{ } + B{ }), 3> {
    return { a + b.x, a + b.y, a + b.z };
}

template <typename A, typename B>
auto operator-(vec<A, 3> const& a, vec<B, 3> const& b) -> vec<decltype(A{ } - B{ }), 3> {
    return { a.x - b.x, a.y - b.y, a.z - b.z };
}

template <typename A, typename B>
auto operator-(vec<A, 3> const& a, B const& b) -> vec<decltype(A{ } - B{ }), 3> {
    return { a.x - b, a.y - b, a.z - b };
}

template <typename A, typename B>
auto operator+(A const& a, vec<B, 3> const& b) -> vec<decltype(A{ } - B{ }), 3> {
    return { a - b.x, a - b.y, a - b.z };
}

template <typename A, typename B>
auto operator*(vec<A, 3> const& a, B const& b) -> vec<decltype(A{ } * B{ }), 3> {
    return { a.x * b, a.y * b, a.z * b };
}

template <typename A, typename B>
auto operator*(A const& a, vec<B, 3> const& b) -> vec<decltype(A{ } * B{ }), 3> {
    return { a * b.x, a * b.y, a * b.z };
}

template <typename A, typename B>
auto operator/(vec<A, 3> const& a, B const& b) -> vec<decltype(A{ } / B{ }), 3> {
    return { a.x / b, a.y / b, a.z / b };
}

template <typename A, typename B>
auto dot(vec<A, 3> const& a, vec<B, 3> const& b) -> decltype(A{ } + B{ }) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

template <typename A, typename B>
auto cross(vec<A, 3> const& a, vec<B, 3> const& b) -> vec<decltype(A{ } + B{ }), 3> {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

template <typename T>
vec<T, 3> max(vec<T, 3> const& a, vec<T, 3> const& b) {
    return {
        std::max(a.x, b.x),
        std::max(a.y, b.y),
        std::max(a.z, b.z)
    };
}

template <typename T>
vec<T, 3> min(vec<T, 3> const& a, vec<T, 3> const& b) {
    return {
        std::min(a.x, b.x),
        std::min(a.y, b.y),
        std::min(a.z, b.z)
    };
}


template <typename A, typename B, typename P, size_t DIM>
auto lerp(vec<A, DIM> const& a, vec<B, DIM> const& b, P const& param) {
    return a + (b - a) * param;
}

template <typename T, size_t DIM>
auto sqr(vec<T, DIM> const& v) {
    return dot(v, v);
}

template <typename T, size_t DIM>
auto norm(vec<T, DIM> const& v) {
    return T(SDL_sqrt(sqr(v)));
}

template <typename T, size_t DIM>
auto normalized(vec<T, DIM> const& v) {
    return v / norm(v);
}
