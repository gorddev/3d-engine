#pragma once

#include <type_traits>

#include "glm/fwd.hpp"
#include "rendering/glPrimitives/glVertex.hpp"
#include "rendering/glPrimitives/glVertexBuffer.hpp"

namespace gan::gl {

    /// @param vertices Array of vertices used to create a buffer
    /// @param num_vertices Number of vertices you use
    /// @param indices Aray of indices indicating the ordering of the vertices in the vertex array
    /// @param num_indices Number of indices in the index array
    /// @return A heap-allocated @code vertexBuffer@endcode
    VertexBuffer createVertexBuffer(
        const vertex3 vertices[],
        uint32_t num_vertices,
        const uint32_t indices[],
        uint32_t num_indices);

    /// @param vertices Array of vertices used to create a buffer
    /// @param num_vertices Number of vertices you use
    /// @param indices Aray of indices indicating the ordering of the vertices in the vertex array
    /// @param num_indices Number of indices in the index array
    /// @return A heap-allocated @code vertexBuffer@endcode
    VertexBuffer createVertexBuffer(
        const vertex2 vertices[],
        uint32_t num_vertices,
        const uint32_t indices[],
        uint32_t num_indices);

    /// Creates a model matrix buffer to avoid CPU bottleneck.
    /// @param modelMatrices The raw data of modelMatrices you want to upload
    /// @param num_modelMatrices The number of model Matrices you want to upload
    /// @return The buffer created by the upload.
    GLuint createModelMatrixBuffer(
        const glm::mat4 modelMatrices[],
        uint32_t num_modelMatrices);

    /// Destroys the vertexArrays and vertex & index buffer in the @code vertexBuffer@endcode object.
    /// @param vb The vertex buffer you want destroyed.
    void destroyVertexBuffer(const VertexBuffer& vb);
}
