// ReSharper disable CppZeroConstantCanBeReplacedWithNullptr
#include <rendering/glFunctions/glBuffers.hpp>
#include <OpenGL.h>
#include <rendering/glFunctions/glMacros.hpp>
#include <cstddef>
#include <glm/mat4x4.hpp>


using namespace gan::gl;

template<typename V>
    requires(std::is_same_v<vertex3, V> || std::is_same_v<V, vertex2>)
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

    vb.indexCount = num_indices;
    // binds the 0 vertex array.
    glBindVertexArray(0); //< now we make sure to unbind our buffer.
    return vb;
}


VertexBuffer gan::gl::createVertexBuffer(
        const vertex3 vertices[],
        const uint32_t num_vertices,
        const uint32_t indices[],
        const uint32_t num_indices)
{
    return initializeVertexBuffer<vertex3>(vertices, num_vertices, indices, num_indices);
}


VertexBuffer gan::gl::createVertexBuffer(
        const vertex2 vertices[],
        const uint32_t num_vertices,
        const uint32_t indices[],
        const uint32_t num_indices)
{
    // create our vertex buffer object
    VertexBuffer vb;
    // get our stride
    constexpr GLsizei stride = sizeof(vertex2) + sizeof(glm::mat4);


    // now we tie this vertex data to the shader.
    // first we specify position 1 -> position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
        stride, nullptr);
    glEnableVertexAttribArray(0);
    // next we specify position 2 -> uv
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
        stride, reinterpret_cast<void*>(offsetof(vertex2, u)));
    glEnableVertexAttribArray(1);
    // finally get color
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE,
        stride, reinterpret_cast<void*>(offsetof(vertex2, r)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0); //< now we make sure to unbind our buffer.
    // update our vertex pointer
    vb.indexCount = num_indices;
    // return our vertexBuffer object.
    return vb;
}



void gan::gl::destroyVertexBuffer(const VertexBuffer& vb) {
    glDeleteVertexArrays(1, &vb.vao); //< Destroy the vertex array
    glDeleteBuffers(1, &vb.vbo); //< Delete the vertex buffer
    glDeleteBuffers(1, &vb.ebo); //< Delete the index buffer
}