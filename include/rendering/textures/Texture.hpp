#pragma once

#include <optional>
#include <glm/fwd.hpp>
#include <types/enums/RenderingSettings.hpp>
#include <core/debug/gan_log.hpp>
#include <OpenGL.h>

/* Created by Gordie Novak on 3/11/26.
 * Purpose: 
 * Creates & stores texture information into one convenient object.*/

namespace gan {

    using GLuint = uint32_t;
    using texID = uint32_t;


    /// Holds texture handle for a texture managed by OpenGL
    struct Texture {
        texID tex_id;   ///< Texture identifier CPU-side in the TextureRegister (texRg)
        GLuint gl_id;   ///< Texture identifier GPU-side in OpenGL
        uint32_t w, h;

    /* ******************************************************************************************** */

        /** @param pixels Raw pixels representing the image
         * @param w Width of the image
         * @param h Height of the image
         * @param scaleMode How the texture appears when scaled (linear vs. nearest neighbor)
         * @param tex_id The position in the textureRegister vector of this particular texture.
         * @param texFormat The format of the texture in the GPU (automatically RGBA)
         * @param pixelType The format of the pixels being uploaded (automatically GL_UNSIGNED_BYTE)
         * @return An optional texture object.
         * @warning The @code tex_id@endcode parameter is not initialized, and must be done afterwords
         */
        static std::optional<Texture> make(
            const void *pixels,
            const uint32_t w,
            const uint32_t h,
            const ScaleMode scaleMode,
            const texID tex_id,
            const GLuint texFormat = GL_RGBA, //< GL Macro for rgba
            const GLuint pixelType = GL_UNSIGNED_BYTE)
        noexcept {
            // create an error handler.
            GLenum error{};
            if ((error = glGetError()) != GL_NO_ERROR) {
                GAN_WriteLog("gl::generate2DTexture()", "There is already an OpenGL error"
                " in the error buffer. Will not proceed with texture generation. GL_ERROR: ", error);
                return std::nullopt;
            }
            // first create a texture id.
            GLuint gl_id;
            // then create the texture in GL
            glGenTextures(1, &gl_id);
            // Bind the texture to the texture id.
            glBindTexture(GL_TEXTURE_2D, gl_id);

            if (scaleMode == GAN_PIXEL) {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            } else if (scaleMode == GAN_LINEAR) {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            }

            if ((error = glGetError()) != GL_NO_ERROR) {
                GAN_WriteLog("gl::generate2DTexture()",
                    "Texture parameter initialization failed.\nRefer to GL Error Code: " ,
                    static_cast<int>(error) , "\n");
                return std::nullopt;
            }

            // then we generate the underlying texture
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                static_cast<int>(texFormat),
                static_cast<int>(w), static_cast<int>(h),
                0,
                texFormat,
                pixelType,
                pixels
            );
            glGenerateMipmap(GL_TEXTURE_2D);

            if ((error = glGetError()) != GL_NO_ERROR) {
                GAN_WriteLog("gl::generate2DTexture()",
                    "Texture generation failed.\nRefer to GL Error Code: ", static_cast<int>(error), "\n");
                return std::nullopt;
            }
            // now we can return our texture id.
            return Texture{tex_id, gl_id, w, h};
        }

        /** Deallocates the texture OpenGL-side, and removes handles
            @param texture The texture you want destroyed */
        static void destroy(Texture& texture) {
            glDeleteTextures(1, &texture.tex_id);
            texture = Texture{0,0,0,0};
        }

    };

}
