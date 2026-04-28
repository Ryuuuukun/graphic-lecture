#pragma once

#include <algorithm>
#include <type_traits>
#include <array>
#include <functional>

template <typename T, size_t DIM>
struct mat {
    using value_type = T;

    using iterator       = T*;
    using const_iterator = T const*;

    using pointer       = T*;
    using const_pointer = T const*;

    using reference       = T&;
    using const_reference = T const&;

    std::array<T, DIM * DIM> buffer;

public:
    mat() = default;

    mat(std::array<T, DIM> buffer)
        : buffer(buffer)
    {}

    mat(std::initializer_list<T> buffer)
        : buffer(buffer)
    {}

    template <typename It>
    mat(It begin, It end)
        : buffer(begin, end)
    {}

    template <typename V> requires std::is_convertible_v<V, T>
    mat(mat<V, DIM> const& other)
        : buffer(other.buffer)
    {}

    pointer data() {
        return buffer.data();
    }

    const_pointer data() const {
        return buffer.data();
    }

    iterator begin() {
        return buffer.begin();
    }

    iterator end() {
        return buffer.end();
    }

    const_iterator begin() const {
        return buffer.begin();
    }

    const_iterator end() const {
        return buffer.end();
    }

    reference operator()(size_t i, size_t j) {
        return buffer[i * DIM + j];
    }

    const_reference operator()(size_t i, size_t j) const {
        return buffer[i * DIM + j];
    }

    mat& operator+=(mat const& other) {
        std::transform(begin(), end(), other.begin(), begin(), std::plus<T>{ });
        return *this;
    }

    mat& operator-=(mat const& other) {
        std::transform(begin(), end(), other.begin(), begin(), std::minus<T>{ });
        return *this;
    }

    mat& operator*=(T const& s) {
        std::transform(begin(), end(), [s](auto& e) { return e * s; });
        return *this;
    }

    mat& operator/=(T const& s) {
        std::transform(begin(), end(), [s](auto& e) { return e * s; });
        return *this;
    }

    mat operator+() const {
        return mat(buffer);
    }

    mat operator-() const {
        return mat(buffer) *= -1;
    }
};


using mat2f = mat<float, 2>;
using mat2d = mat<double, 2>;

using mat3f = mat<float, 3>;
using mat3d = mat<double, 3>;

using mat4f = mat<float, 4>;
using mat4d = mat<double, 4>;


template <typename A, typename B, size_t DIM>
auto operator+(mat<A, DIM> const& a, mat<B, DIM> const& b) -> mat<decltype(A{ } + B{ }), DIM> {
    return mat{ a } += b;
}

template <typename A, typename B, size_t DIM>
auto operator-(mat<A, DIM> const& a, mat<B, DIM> const& b) -> mat<decltype(A{ } - B{ }), DIM> {
    return mat{ a } -= b;
}

template <typename A, typename B, size_t DIM>
auto operator*(mat<A, DIM> const& a, B const& b) -> mat<decltype(A{ } * B{ }), DIM> {
    return mat{ a } *= b;
}

template <typename A, typename B, size_t DIM>
auto operator*(A const& a, mat<B, DIM> const& b) -> mat<decltype(A{ } * B{ }), DIM> {
    return mat{ b } *= a;
}

template <typename A, typename B, size_t DIM>
auto operator/(mat<A, DIM> const& a, B const& b) -> mat<decltype(A{ } / B{ }), DIM> {
    return mat{ a } /= b;
}