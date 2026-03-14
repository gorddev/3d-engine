#pragma once

#include <cstdint>
#include <OpenGL.h>
#include <rendering/render-primitives/Vertex.hpp>
#include "core/debug/gan_log.hpp"

// Created by Gordie Novak on 2/17/26.

// vertex buffers hold all the information needed
// to render an object that has already been added
// to open gl.

typedef uint32_t GLuint; //< typedef so we can use w/o includes.

namespace gan {
    class Renderer;
    class ShapeFactory;
    class EngineCore;
}

namespace gan {

    /// A vertex buffer is an internal engine object
    /// that stores internal vertex and rendering information
    /// for openGL rendering.
    struct VertexBuffer {
        GLuint vao;     ///< Vertex Array handle
        GLuint vbo;     ///< Vertex Buffer handle
        GLuint mbo;     ///< Matrix buffer handle
        GLuint cbo;     ///< Color buffer handle.
        uint32_t vertexCount;  ///< Number of indices

    private:
    /** **************************************************************************** */
        friend class gan::EngineCore;
        friend class gan::ShapeFactory;
        friend class gan::Renderer;

        /** Makes a VertexBuffer with OpenGL commands
         * @param vertices An array of vertices to upload
         * @param numVertices The number of vertices in the array
         * @return A properly initialized @code VertexBuffer@endcode object.
         */
        static VertexBuffer make(const Vertex vertices[], const size_t numVertices) {
            VertexBuffer buffer{};

            glGenVertexArrays(1, &buffer.vao);
            glBindVertexArray(buffer.vao);

            glGenBuffers(1, &buffer.vbo);
            glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
            glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex),
                vertices, GL_STATIC_DRAW);

            // set up our position (x,y,z)
            glVertexAttribPointer(
                0, 3,
                GL_FLOAT, GL_FALSE, sizeof(Vertex),
                reinterpret_cast<void*>(offsetof(Vertex, x)));
            glEnableVertexAttribArray(0);
            // set up our UVs
            glVertexAttribPointer(
                1, 2,
                GL_FLOAT, GL_FALSE, sizeof(Vertex),
                reinterpret_cast<void*>(offsetof(Vertex, u)));
            // set up our normals (nx, ny, nz)
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(
                2, 3,
                GL_FLOAT, GL_FALSE, sizeof(Vertex),
                reinterpret_cast<void*>(offsetof(Vertex, nx)));
            glEnableVertexAttribArray(2);
            // unbind our vertex array.
            glBindVertexArray(0);

            buffer.vertexCount = numVertices;
            buffer.mbo = 0;
            buffer.cbo = 0;

            return buffer;
        }

    /* **************************************************************************** */

        /** Generates a matrix VBO for a specific vertex buffer object.
         * @param buffer The VertexBuffer you want to attach this upload to
         * @param shaderLoc The location in the shader you want to upload the data to. Must be divisible by 4.
         * @return An optional GLuint: exists if creation was successful
         * @warning shaderLoc must be divisible by four. */
        [[nodiscard]] static err::Errcode genMatrixVBO(VertexBuffer& buffer, size_t shaderLoc) {
            if (shaderLoc % 4 != 0) {
                GAN_WriteLog("VertexBuffer::genMatrixVBO()", "shaderLoc parameter is not valid, as it must"
                                    "be divisible by 4. Provided value: ", shaderLoc);
                return err::ERR;
            }
            glBindVertexArray(buffer.vao);
            glGenBuffers(1, &buffer.mbo); //< generate buffer into our new vbo
            glBindBuffer(GL_ARRAY_BUFFER, buffer.mbo);
            for (size_t i = 0; i < 4; i++) {
                glEnableVertexAttribArray(i + shaderLoc);

                glVertexAttribPointer(
                    shaderLoc + i, 4, GL_FLOAT, GL_FALSE,
                    sizeof(float)*16,   //< size of 4x4 float matrix
                    reinterpret_cast<void*>(sizeof(float)*4*i));    //< offset we render to

                glVertexAttribDivisor(shaderLoc + i, 1);
            }
            return err::OK;
        }

    /* **************************************************************************** */

        [[nodiscard]] static err::Errcode genColorVBO(VertexBuffer& buffer, size_t shaderLoc) {
            if (shaderLoc <= 2) {
                GAN_WriteLog("VertexBuffer::genColorVBO()",
                    "shaderLoc parameter is not valid, as it"
                    "overrides the pos, uv, and norm shader locations"
                    "which range from 1-2. Provided shaderLoc: ", shaderLoc);
                return err::ERR;
            }
            glBindVertexArray(buffer.vao);
            glGenBuffers(1, &buffer.cbo);
            glBindBuffer(GL_ARRAY_BUFFER, buffer.cbo);
            glVertexAttribPointer(
                shaderLoc, 1,
                GL_FLOAT, GL_FALSE, sizeof(float)*4,
                nullptr);
            glEnableVertexAttribArray(shaderLoc);
            return err::OK;
        }

    /* **************************************************************************** */

        /** Destroys the underlying vertex buffer on the OpenGL side and sets values to 0. */
        static void destroy(VertexBuffer& buffer) {
            glDeleteVertexArrays(1, &buffer.vao);
            glDeleteBuffers(1, &buffer.vbo);
            buffer = VertexBuffer{0, 0, 0};
        }
    };


}