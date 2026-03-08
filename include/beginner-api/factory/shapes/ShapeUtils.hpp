#pragma once


#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "Colors.hpp"
#include "rendering/glPrimitives/glVertex.hpp"


namespace gan {

    /// Requires that the struct has a valid accessible static vertexCount variable.
    template<typename S> concept struct_has_vertex_count = requires
        { requires std::same_as<decltype(S::numVertices), const size_t>; };
    /// Requires that the struct have a valid accessible static index count
    template<typename S> concept struct_has_index_count = requires
        { requires std::same_as<decltype(S::numIndices), const size_t>; };
    /// Ensures that the shape has a valid vertex generation function.
    template<typename S> concept has_static_vertex_generation_function =
        requires(S s, std::array<gl::vertex3, S::numVertices>& v, glm::vec3 dim )
            { std::invoke(S::vertices,
                std::forward<std::array<gl::vertex3, S::numVertices>&>(v),
                std::forward<glm::vec3>(dim));
            };
    /// Ensures that the shape has a valid static index generation function
    template<typename S> concept has_static_index_generation_function =
        requires(S s, std::array<unsigned int, S::numIndices>& iArr)
        { std::invoke(s.indices, std::forward<std::array<unsigned int, S::numIndices>&>(iArr)); };
    /// Single concept that houses all the requirements for a shape to be valid:
    /// - Must have a valid @code static constexpr size_t numVertices@endcode
    /// - Must have a valid @code static constexpr size_T numIndices@endcode
    /// - Must have a valid @code static vertices(...)@endcode that accepts
    /// an array reference for @code std::array<gl::vertex3, numVertices>@endcode, and
    /// a @code glm::vec3@endcode that specifies it's dimensions
    template<typename S> concept has_shape_requirements =
        struct_has_index_count<S> && struct_has_vertex_count<S> &&
        has_static_vertex_generation_function<S> &&
        has_static_index_generation_function<S>;

}
