#pragma once
#include <type_traits>

namespace gan {
    /** @brief Basic dimension struct that contains a @code short w@endcode and @code short h@endcode.
     * @details Contains @code ==, !=, +, -@endcode nontrivial operator overloads and an @code int area()@endcode function that returns @code w*h@endcode.
     * @note Typically used for 2D object's demensions in a limited game environment world.*/
    struct dim2 {
        short w, h;
        template<typename T>
            requires(std::is_integral_v<T>)
        dim2(T w, T h) : w(w), h(h) {}
        bool operator==(const dim2& d) const {
            return (w == d.w && h == d.h);
        }
        bool operator!=(const dim2& d) const {
            return (w != d.w || h != d.h);
        }
        dim2 operator+(const dim2& d) const {
            return {static_cast<short>(w + d.w), static_cast<short>(h + d.h)};
        }
        dim2 operator-(const dim2& d) const {
            return {static_cast<short>(w - d.w), static_cast<short>(h - d.h)};
        }
        void operator+=(const dim2& d) {
            w += d.w;
            h += d.h;
        }
        void operator-=(const dim2& d) {
            w -= d.w;
            h -= d.h;
        }
        dim2 operator-() const {
            return {-w, -h};
        }
        int area() const { return w*h; }

        dim2 operator*(int i) const {
            return {w*2, h*2};
        }
    };
}
