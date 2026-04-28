#pragma once

#include <algorithm>
#include <cstdint>

template <typename T>
struct color {
    T r;
    T g;
    T b;

public:
    color() = default;

    color(T v)
        : r(v), g(v), b(v)
    {}

    color(T r, T g, T b)
        : r(r), g(g), b(b)
    {}

    template <typename V> requires std::is_convertible_v<V, T>
    color(color<V> const& other)
        : r(other.r), g(other.g), b(other.b)
    {}

    static color from_rgba(uint32_t color) {
        return {
            static_cast<T>((color & 0xFF000000) >> 24),
            static_cast<T>((color & 0x00FF0000) >> 16),
            static_cast<T>((color & 0x0000FF00) >> 8)
        };
    }

    static color from_argb(uint32_t color) {
        return {
            static_cast<T>((color & 0x00FF0000) >> 16),
            static_cast<T>((color & 0x0000FF00) >> 8),
            static_cast<T>(color & 0x000000FF)
        };
    }

    uint32_t to_rgba(uint8_t alpha = 0xFF) const {
        return std::clamp(static_cast<uint32_t>(r), 0u, 255u) << 24 |
               std::clamp(static_cast<uint32_t>(g), 0u, 255u) << 16 |
               std::clamp(static_cast<uint32_t>(b), 0u, 255u) << 8  | alpha;
    }

    uint32_t to_argb(uint8_t alpha = 0xFF) const {
        return std::clamp(static_cast<uint32_t>(r), 0u, 255u) << 16 |
               std::clamp(static_cast<uint32_t>(g), 0u, 255u) << 8  |
               std::clamp(static_cast<uint32_t>(b), 0u, 255u)       | alpha << 24;

    }

    color& operator+=(color const& o) {
        r += o.r; g += o.g; b += o.b;
        return *this;
    }

    color& operator+=(T const& s) {
        r += s; g += s; b += s;
        return *this;
    }

    color& operator-=(color const& o) {
        r -= o.r; g -= o.g; b -= o.b;
        return *this;
    }

    color& operator-=(T const& s) {
        r -= s; g -= s; b -= s;
        return *this;
    }

    color& operator*=(color const& o) {
        r *= o.r; g *= o.g; b *= o.b;
        return *this;
    }

    color& operator*=(T const& s) {
        r *= s; g *= s; b *= s;
        return *this;
    }

    color& operator/=(color const& o) {
        r /= o.r; g /= o.g; b /= o.b;
        return *this;
    }

    color& operator/=(T const& s) {
        r /= s; g /= s; b /= s;
        return *this;
    }
};


using color3f = color<float>;
using color3d = color<double>;


template <typename X, typename Y>
auto operator+(color<X> const& x, color<Y> const& y) {
    return color(x.r + y.r, x.g + y.g, x.b + y.b);
}

template <typename X, typename Y>
auto operator+(color<X> const& x, Y const& y) {
    return color(x.r + y, x.g + y, x.b + y);
}

template <typename X, typename Y>
auto operator+(X const& x, color<Y> const& y) {
    return color(x + y.r, x + y.g, x + y.b);
}

template <typename X, typename Y>
auto operator-(color<X> const& x, color<Y> const& y) {
    return color(x.r - y.r, x.g - y.g, x.b - y.b);
}

template <typename X, typename Y>
auto operator-(color<X> const& x, Y const& y) {
    return color(x.r - y, x.g - y, x.b - y);
}

template <typename X, typename Y>
auto operator-(X const& x, color<Y> const& y) {
    return color(x - y.r, x - y.g, x - y.b);
}

template <typename X, typename Y>
auto operator*(color<X> const& x, color<Y> const& y) {
    return color(x.r * y.r, x.g * y.g, x.b * y.b);
}

template <typename X, typename Y>
auto operator*(color<X> const& x, Y const& y) {
    return color(x.r * y, x.g * y, x.b * y);
}

template <typename X, typename Y>
auto operator*(X const& x, color<Y> const& y) {
    return color(x * y.r, x * y.g, x * y.b);
}

template <typename X, typename Y>
auto operator/(color<X> const& x, color<Y> const& y) {
    return color(x.r / y.r, x.g / y.g, x.b / y.b);
}

template <typename X, typename Y>
auto operator/(color<X> const& x, Y const& y) {
    return color(x.r / y, x.g / y, x.b / y);
}

template <typename X, typename Y>
auto operator/(X const& x, color<Y> const& y) {
    return color(x / y.r, x / y.g, x / y.b);
}

static constexpr float LUMA_RED   = 0.212;
static constexpr float LUMA_GREEN = 0.715;
static constexpr float LUMA_BLUE  = 0.072;

template <typename T>
auto luma(color<T> const& c) {
    return c.r * LUMA_RED + c.g * LUMA_GREEN + c.b * LUMA_BLUE;
}

template <typename T>
auto luma(uint32_t value) {
    return luma(color<T>::from_argb(value));
}

template <typename T, typename P>
auto gamma(color<T> const& c, P const& param) {
    return color<T>(
        SDL_pow(c.r / T(255), param) * T(255),
        SDL_pow(c.g / T(255), param) * T(255),
        SDL_pow(c.b / T(255), param) * T(255)
    );
}

template <typename A, typename B, typename P>
auto lerp(color<A> const& a, color<B> const& b, P const& param) {
    return a + (b - a) * param;
}

template <typename T>
auto clamp(color<T> const& c, T min, T max) {
    return color<T>(
        std::clamp(c.r, min, max),
        std::clamp(c.g, min, max),
        std::clamp(c.b, min, max)
    );
}

template <typename X, typename Y>
auto max(color<X> const& x, color<Y> const& y) {
    return color(
        std::max(x.r, y.r),
        std::max(x.g, y.g),
        std::max(x.b, y.b)
    );
}

template <typename X, typename Y>
auto min(color<X> const& x, color<Y> const& y) {
    return color(
        std::min(x.r, y.r),
        std::min(x.g, y.g),
        std::min(x.b, y.b)
    );
}

