#include <iostream>
#include <OpenGL.h>
#include <ostream>
#include <rendering/glFunctions/glTextures.hpp>

#include "../../../include/core/debug/gan_err.hpp"
#include "types/enums/RenderingSettings.hpp"

using namespace gan::gl;

std::optional<GLuint> gan::gl::generate2DTexture(
    const void* pixels,
    const uint32_t w,
    const uint32_t h,
    const ScaleMode scaleMode,
    const GLuint format,
    const GLuint type)
{
    // create an error handler.
    GLenum error{};
    while ((error = glGetError()) != GL_NO_ERROR) {}
    // first create a texture id.
    GLuint texture_id;
    // then create the texture in GL
    glGenTextures(1, &texture_id);
    // Bind the texture to the texture id.
    glBindTexture(GL_TEXTURE_2D, texture_id);

    if (scaleMode == PIXEL) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    } else if (scaleMode == GAN_LINEAR) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    if ((error = glGetError()) != GL_NO_ERROR) {
        GAN_WriteError("gl::generate2DTexture()",
            "Texture parameter initialization failed.\nRefer to GL Error Code: " ,
            static_cast<int>(error) , "\n");
        return std::nullopt;
    }



    // then we generate the underlying texture
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        format,
        w, h,
        0,
        format,
        type,
        pixels
    );

    if ((error = glGetError()) != GL_NO_ERROR) {
        GAN_WriteError("gl::generate2DTexture()",
            "Texture generation failed.\nRefer to GL Error Code: ", static_cast<int>(error), "\n");
        return std::nullopt;
    }
    // now we can return our texture id.
    return texture_id;
}
