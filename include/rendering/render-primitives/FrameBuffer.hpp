#pragma once
#include <cstdint>
#include <optional>
#include <OpenGL.h>
#include <glbinding/gl/enum.h>

#include "VertexBuffer.hpp"
#include "rendering/textures/Texture.hpp"
#include "types/math/dim2.hpp"
#include "types/math/fRect.hpp"
#include "types/math/rect.hpp"


/* Created by Gordie Novak on 3/13/26.
 * Purpose: 
 * Stores a writable texture (a framebuffer) so for each scene, or shadow obj,
 * you can write to a framebuffer, and then scale it up to the window!
 */

using GLuint = uint32_t;
using GLint = int32_t;

namespace gan {
    struct VertexBuffer;

    struct FrameBuffer {

        GLuint fbo;     ///< Handle to the GL Framebuffer
        GLuint dbo;     ///< Handle to the depth buffer for depth testing.
        Texture tex;    ///< Texture we write to with the FrameBuffer
        ScaleMode scaleMode;

    /* ******************************************************************************************** */

        /**
         * @param w The width of the framebuffer you want to generate
         * @param h The height of the framebuffer you want to generate
         * @param mode The scale mode of the framebuffer generated
         * @param tex_id Optional: a texutre id to assign to the internal tex object.
         * @return A Framebuffer if creation was successful. An @code std::nullopt@endcode otherwise. */
        static std::optional<FrameBuffer> make(uint32_t w, uint32_t h, ScaleMode mode, texID tex_id = 0) noexcept {
            if (w == 0 || h == 0) {
                GAN_WriteLog("FrameBuffer::make()", "Width and height parameters cannot be zero.");
                return std::nullopt;
            } if (w > std::numeric_limits<int16_t>::max() || h > std::numeric_limits<int16_t>::max()) {
                GAN_WriteLog("FrameBuffer::make()", "Width and height parameters cannot be greater than ",
                    std::numeric_limits<int16_t>::max());
                return std::nullopt;
            }

            FrameBuffer fb{};

            glGenFramebuffers(1, &fb.fbo);
            glBindFramebuffer(GL_FRAMEBUFFER, fb.fbo);

            // attempt to make a texture
            auto texOpt = Texture::make(nullptr, w, h, mode, tex_id);
            if (!texOpt) {
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                GAN_WriteLog("FrameBuffer::make()", "Failed to create texture for framebuffer.");
                return std::nullopt;
            }
            fb.tex = texOpt.value();
            // attach texture to our bound framebuffer
            glFramebufferTexture2D(
                GL_FRAMEBUFFER,
                GL_COLOR_ATTACHMENT0,
                GL_TEXTURE_2D,
                fb.tex.gl_id,
                0
            );

            // now we create a depth buffer.
            glGenRenderbuffers(1, &fb.dbo);
            glBindRenderbuffer(GL_RENDERBUFFER, fb.dbo);
            // keeps track of fragment position
            glRenderbufferStorage(
                GL_RENDERBUFFER,
                GL_DEPTH24_STENCIL8,
                w, h // NOLINT(*-narrowing-conversions)
            );
            // actually attach the render buffer.
            glFramebufferRenderbuffer(
                GL_FRAMEBUFFER,
                GL_DEPTH_STENCIL_ATTACHMENT,
                GL_RENDERBUFFER,
                fb.dbo
            );

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                GAN_WriteLog("FrameBuffer::make()",
                    "Failed to create framebuffer as framebuffer is not marked as complete.\n"
                    "OpenGL Error Code: ", glGetError());
                return std::nullopt;
            }

            fb.scaleMode = mode;
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            return fb;
        }

        static void bind(FrameBuffer& fb) noexcept {
            glBindFramebuffer(GL_FRAMEBUFFER, fb.fbo);
            glViewport(0, 0, fb.tex.w, fb.tex.h);
        }

        /** @warning Does not bind the frameBuffer to blit to initially.  */
        static void blit(const FrameBuffer& fb, const rect& dst) {
            glBlitFramebuffer(
                0, 0, fb.tex.w, fb.tex.h, //< src
                dst.x, dst.y, dst.w, dst.h, //< dst
                GL_COLOR_BUFFER_BIT, fb.scaleMode
            );
        }

        /** Draws the FrameBuffer directly to a bound VertexBuffer & uploads raw vertex data.
         * Significantly more dynamic than the blit (because it allows for the use of shaders)
         * but also requires setup (like setting up a VertexBuffer beforehand.*/
        static void draw(const FrameBuffer& fb, const VertexBuffer& vb, const fRect& dst) {
            const float vertices[16] = {
                dst.x,  dst.y,  0.0f, 1.0f, // Top-left vertex (position, texture coord)
                dst.x + dst.w,  dst.y,  1.0f, 1.0f, // Top-right
                dst.x, dst.y + dst.h,  0.0f, 0.0f, // Bottom-left
                dst.x + dst.w, dst.y + dst.h,  1.0f, 0.0f  // Bottom-right
            };

            glBindBuffer(GL_ARRAY_BUFFER, vb.vbo);
            glBufferData(
                GL_ARRAY_BUFFER,
                sizeof(vertices),
                vertices,
                GL_DYNAMIC_DRAW
            );

            glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        static void destroy(FrameBuffer& fb) {
            Texture::destroy(fb.tex);
            glDeleteRenderbuffers(1, &fb.dbo);
            glDeleteFramebuffers(1, &fb.fbo);
            fb.dbo = 0;
            fb.fbo = 0;
        }
    };

}


