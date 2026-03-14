#pragma once

namespace gan {
    /// Primary structure used for rendering 3d textured objects.
    /// "3c" stands for 3d + textured.
    struct Vertex {
        float x, y, z;
        float u, v;
        float nx, ny, nz;

        // Static initialization functions.
        static Vertex make(float x, float y, float z) { return Vertex{x,y,z,0,0,0,0,0};}
        static Vertex make(float x, float y, float z, float u, float v) { return Vertex{x,y,z,u,v,0,0,0}; }
        static Vertex make(float x, float y, float z, float u, float v, float nx, float ny, float nz) { return Vertex{x,y,z,u,v,nx,ny,nz}; }
    };
}
