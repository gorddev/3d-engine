#pragma once

#include "rendering/render-primitives/VertexBuffer.hpp"
#include "rendering/shaders/ShaderManager.hpp"
#include "shapes/raw-shapes/Box3DRaw.hpp"

// made by gordie novak feb 21st, 2023

// the primitive factory makes various shapes that can be automatically
// added to the engine or any user system they want them for.

namespace gan {

    class ShapeFactory final {

        /// Needs to reference the shader manager to initialize objects with shaders
        const ShaderHandler& shaderManager;
        /// Internal vector to destroy/reuse vertex buffers.
        std::vector<VertexBuffer> vertexBuffers;

        /// Gets an element specific VertexBuffer
        /// @tparam ShapeT The Shape you want the associated vertex buffer for.
        /// @return A vertex buffer appropriate for said shape.
        template<typename ShapeT>
        VertexBuffer getShapeVertexBuffer() {
            static size_t buffer_id = vertexBuffers.size(); //< Static permanent reference to position in the vector.
            if (buffer_id >= vertexBuffers.size())
                vertexBuffers.resize(buffer_id + 1);
            else if (vertexBuffers[buffer_id].vao != 0)
                return vertexBuffers[buffer_id];
            vertexBuffers[buffer_id] = VertexBuffer::make(
                ShapeT::SisterType::vertices(), ShapeT::SisterType::numVertices);
            return vertexBuffers[buffer_id];
        }

        /// Gets an element specific VertexBuffer
        /// @tparam ShapeT The Shape you want the associated vertex buffer for.
        /// @return A vertex buffer appropriate for said shape.
        template<typename ShapeT, size_t Detail>
            requires(ShapeT::DefaultDetail > 4)
        VertexBuffer getShapeVertexBuffer() {
            static size_t buffer_id = vertexBuffers.size(); //< Static permanent reference to position in the vector.
            if (buffer_id >= vertexBuffers.size())
                vertexBuffers.resize(buffer_id + 1);
            else if (vertexBuffers[buffer_id].vao != 0)
                return vertexBuffers[buffer_id];
            vertexBuffers[buffer_id] = VertexBuffer::make(ShapeT::template SisterType<Detail>::vertices(),
              ShapeT::template SisterType<Detail>::numVertices);
            return vertexBuffers[buffer_id];
        }

        friend class Book;
        explicit ShapeFactory(const ShaderHandler& shaderManager)
            : shaderManager(shaderManager) {
            static size_t count = 0;
            if (count != 0) {
                throw std::runtime_error("Cannot make more than one ShapeFactory.");
            }
            count++;
        }
    public:

        /// @tparam ShapeT The type of shape you want to create.
        /// @param color The color of the shape you want to create.
        /// @param args The arguments needed to construct the shape.
        /// @return A handler to the shape that lets you modify its properties.
        template<typename ShapeT, typename... Args>
            requires( !(requires(ShapeT t) {ShapeT::DefaultDetail;}))
        ShapeT make(Args&&... args) {
            // then form the object.
            return ShapeT({
                .vb = getShapeVertexBuffer<ShapeT>(),
                .mesh {.shader = shaderManager.getDefault3DShader()},
                .t {.scale = ShapeT::SisterType::scaleFactor(std::forward<Args>(args)...)},
            });
        }

        /// @tparam ShapeT The type of shape you want to create.
        /// @tparam Detail The level of detail the shape should be drawn to. (Higher detail = higher vertex count)
        /// @param args The arguments needed to construct the shape.
        /// @return A handler to the shape that lets you modify its properties.
        template<typename ShapeT, size_t Detail = ShapeT::DefaultDetail, typename... Args>
            requires(Detail <= 40)
        ShapeT make(Args&&... args) {
            // First, create an array of the size of the sister type.
            auto vb = getShapeVertexBuffer<ShapeT, Detail>();
            // then form the object.
            return ShapeT({
                .vb = vb,
                .mesh {.shader = shaderManager.getDefault3DShader()},
                .t {.scale = ShapeT::template SisterType<Detail>::scaleFactor(std::forward<Args>(args)...)},
            });
        }

        ~ShapeFactory() {
            for (auto& vb : vertexBuffers) {
                VertexBuffer::destroy(vb);
            }
        }
    };


}