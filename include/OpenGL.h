#pragma once

// Made by gordie on an unknown date.
// Ensures that we use the correct OpenGL on each platform.
// Currently only implemented for __APPLE__ and __EMSCRIPTEN__

#ifdef __APPLE__
    #define GLFW_INCLUDE_GLCOREARB
    #include <GLFW/glfw3.h>
#elifdef __EMSCRIPTEN__
    #include <GLES3/gl3.h>
    #ifndef GL_WRITE_ONLY
    #define GL_WRITE_ONLY 0x88B9
    #endif
#elifdef __linux__
    #include <GLES3/gl3.h>
#endif

// GL_CHECK function which just polls all the errors and returns the error code, line, & file.
#define GL_CHECK() \
do { \
GLenum err = glGetError(); \
if (err != GL_NO_ERROR) \
std::cerr << "GL error: " << err << " at " << __LINE__ << " in " << __FILE_NAME__ << std::endl; \
} while (0)