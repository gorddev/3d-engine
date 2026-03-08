#pragma once

namespace gan::gl {
    /// Structure used for rendering 3d objects without color.
    /// "vertex3depr" stands for "3-dimensional vertex"
    struct vertex3depr {
        float x, y, z;
        float r, g, b, a;
        vertex3depr() = default;
        vertex3depr(float x, float y, float z)
            : x(x), y(y), z(z), r(1.0), g(1.0), b(1.0), a(1.0) {}
        vertex3depr(float x, float y, float z, float r, float g, float b, float a)
            : x(x), y(y), z(z), r(r), g(g), b(b), a(a) {}
    };

    /// Primary structure used for rendering 3d textured objects.
    /// "3c" stands for 3d + textured.
    struct vertex3 {
        float x, y, z;
        float u, v;
        float nx, ny, nz;
    };

    /// Primary structure used for rendering 2d objects.
    /// "vertex2c" stands for "2-dimensional vertex + color"
    struct vertex2 {
        float x, y;
        float u, v;
        float r, g, b, a;
        vertex2() = default;
        vertex2(float x, float y, float u, float v)
            : x(x), y(y), u(u), v(v), r(1.0), g(1.0), b(1.0), a(1.0) {}
        vertex2(float x, float y, float u, float v, float r, float g, float b, float a)
            : x(x), y(y), u(u), v(v), r(r), g(g), b(b), a(a) {}
    };

    /// Secondary structure used for rendering 2d objects without color modification.
    /// "vertex2" stands for "2-dimensional vertex"
    struct vertex2depr {
        float x, y;
        float r, g, b, a;
        vertex2depr() = default;
        vertex2depr(float x, float y)
            : x(x), y(y), r(1.0), g(1.0), b(1.0), a(1.0) {}
        vertex2depr(float x, float y, float r, float g, float b, float a)
            : x(x), y(y), r(r), g(g), b(b), a(a) {}
    };


}