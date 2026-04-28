#pragma once

#include <concepts>
#include <functional>

#include "qut.h"
#include "vec.h"


template <typename T, size_t DIM>
using affine = std::function<vec<T, DIM>(vec<T, DIM>)>;

using affine2f = affine<float, 2>;
using affine2d = affine<double, 2>;

using affine3f = affine<float, 3>;
using affine3d = affine<double, 3>;

namespace fin {

    template <typename X, typename Y>
    struct affine_pipe;

    struct affine_adapter {
        // template <typename V, typename A>
        // requires is_vector<V> && std::derived_from<A, affine_adapter>
        // friend auto operator|(V v, A&& a) {
        //     return std::forward<A>(a)(v);
        // }
        //
        // template <typename X, typename Y>
        // requires std::derived_from<X, affine_adapter> && std::derived_from<Y, affine_adapter>
        // friend auto operator|(X&& x, Y&& y) {
        //     return affine_pipe{ std::forward<X>(x), std::forward<Y>(y) };
        // }
    };

    template <typename A>
    concept is_affine = std::derived_from<std::remove_cvref_t<A>, affine_adapter>;

    template <is_vector V, is_affine A>
    auto operator|(V v, A&& a) {
        return std::forward<A>(a)(v);
    }

    template <is_affine X, is_affine Y>
    auto operator|(X&& x, Y&& y) {
        return affine_pipe<X, Y>{ std::forward<X>(x), std::forward<Y>(y) };
    }

    template <is_affine X, is_affine Y>
    struct affine_pipe<X, Y> : affine_adapter {
        X x;
        Y y;

    public:
        affine_pipe(X&& x, Y&& y)
            : x(std::forward<X>(x)), y(std::forward<Y>(y))
        {}

        template <is_vector V>
        auto operator()(V v) const {
            return y(x(v));
        }
    };


    template <typename T, size_t DIM>
    struct translate_adapter : affine_adapter {
        vec<T, DIM> delta;

    public:
        translate_adapter(vec<T, DIM> delta)
            : delta(delta)
        {}

        template <is_vector V>
        auto operator()(V&& v) const {
            return v + delta;
        }
    };

    template <typename T>
    auto translate(T x, T y) {
        return translate_adapter<T, 2>({ x, y });
    }

    template <typename T>
    auto translate(T x, T y, T z) {
        return translate_adapter<T, 3>({ x, y, z });
    }


    template <typename T, size_t DIM>
    struct rotate_adapter;

    template <typename T>
    struct rotate_adapter<T, 3> : affine_adapter {
        qut<T> quat;

    public:
        rotate_adapter(qut<T> quat)
            : quat(quat)
        {}

        template <is_vector V>
        auto operator()(V v) const {
            return (quat * qut(v) * reciprocal(quat)).v;
        }
    };

    template <typename T>
    auto rotate(T angle, vec<T, 3> axis) {
        return rotate_adapter<T, 3>{ { T(SDL_cos(angle / 2)), T(SDL_sin(angle / 2)) * normalized(axis) } };
    }

    template <typename T>
    auto rotate_x(T angle) {
        return rotate(angle, { 1, 0, 0 });
    }

    template <typename T>
    auto rotate_y(T angle) {
        return rotate(angle, { 0, 1, 0 });
    }

    template <typename T>
    auto rotate_z(T angle) {
        return rotate(angle, { 0, 0, 1 });
    }

}

