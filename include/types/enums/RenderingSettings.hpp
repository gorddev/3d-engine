#pragma once

#include <SDL3/SDL_render.h>
#include <OpenGL.h>

namespace gan {

    /// How textures appear in the engine
    enum ScaleMode : uint_fast32_t {
        PIXEL = GL_NEAREST,
        GAN_LINEAR = GL_LINEAR,
        MIPMAP_PIXEL = GL_NEAREST_MIPMAP_NEAREST,
        MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
    };

    /// Flags for window creation
    enum WindowProperty : uint64_t {
        WindowFlagNone = 0,
        WindowFullscreen = SDL_WINDOW_FULLSCREEN,
        WindowResizable = SDL_WINDOW_RESIZABLE,
        WindowBorderless = SDL_WINDOW_BORDERLESS,
        WindowFloatOnTop = SDL_WINDOW_ALWAYS_ON_TOP,
        WindowKeyboardGrabbed = SDL_WINDOW_KEYBOARD_GRABBED,
        WindowMouseConfined = SDL_WINDOW_MOUSE_GRABBED,
        WindowMouseHidden = SDL_WINDOW_MOUSE_RELATIVE_MODE,
        WindowTransparent = SDL_WINDOW_TRANSPARENT,
        WindowFocused = SDL_WINDOW_INPUT_FOCUS,
    };


    inline void operator&=(WindowProperty& w1, const WindowProperty w2) noexcept {
        w1 = static_cast<WindowProperty>(static_cast<uint64_t>(w1) & w2);
    }
    inline void operator|=(WindowProperty& w1, const WindowProperty w2) noexcept {
        w1 = static_cast<WindowProperty>(static_cast<uint64_t>(w1) | w2);
    }

    inline void operator~(WindowProperty& w) noexcept {
        w = static_cast<WindowProperty>(~static_cast<uint64_t>(w));
    }

    inline WindowProperty operator|(WindowProperty w1, const WindowProperty w2) noexcept {
        return static_cast<WindowProperty>(static_cast<uint64_t>(w1) | w2);
    }




}