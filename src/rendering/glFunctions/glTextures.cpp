#include <iostream>
#include <OpenGL.h>
#include <ostream>
#include <rendering/glFunctions/glTextures.hpp>

#include "../../../include/core/debug/gan_log.hpp"
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

}
