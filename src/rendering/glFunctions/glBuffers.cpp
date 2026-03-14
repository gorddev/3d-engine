// ReSharper disable CppZeroConstantCanBeReplacedWithNullptr
#include <rendering/glFunctions/glBuffers.hpp>
#include <OpenGL.h>
#include <rendering/glFunctions/glMacros.hpp>
#include <cstddef>
#include <glm/mat4x4.hpp>


using namespace gan::gl;

template<typename V>
    requires(std::is_same_v<Vertex, V> || std::is_same_v<V, vertex2>)
VertexBuffer initializeVertexBuffer(
    const V vertices[], uint32_t num_vertices,
    const uint32_t indices[], uint32_t num_indices)
{
    VertexBuffer vb;
    // generate a vertex array object.
    glGenVertexArrays(1, &vb.vao);
    // bind the vertex array object.
    glBindVertexArray(vb.vao);

    glGenBuffers(1, &vb.vbo); //< Generate a vertex buffer
    glGenBuffers(1, &vb.ebo); //< generate an index buffer

    // upload our vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vb.vbo);
    glBufferData(GL_ARRAY_BUFFER, num_vertices * (sizeof(V) + sizeof(glm::mat4)),
             vertices, GL_STATIC_DRAW);

    // upload our index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vb.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(uint32_t),
                 indices, GL_STATIC_DRAW);

    // first we specify position 1 -> position
    glVertexAttribPointer(0, offsetof(V, u)/sizeof(float), GL_FLOAT, GL_FALSE,
        sizeof(V), 0x0);
    glEnableVertexAttribArray(0);
    // next we specify position 2 -> uv
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
        sizeof(V), reinterpret_cast<void*>(offsetof(V, u)));
    glEnableVertexAttribArray(1);
    // finally we specify color -> rgba
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
        sizeof(V), reinterpret_cast<void*>(offsetof(V, nx)));
    glEnableVertexAttribArray(2);

    // binds the 0 vertex array.
    glBindVertexArray(0); //< now we make sure to unbind our buffer.
    return vb;
}