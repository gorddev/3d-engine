
#include <core/debug/gan_io.hpp>

#ifdef GAN_DEBUG
#include <iostream>
#include <ostream>
#endif

#include <rendering/Window.hpp>
#include <types/math/dim2.hpp>
#include <types/enums/RenderingSettings.hpp>
#include <OpenGL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_render.h>

// created by gordie feb 16th. implementation for window

using namespace gan;

Window::Window(const char windowName[], const dim2 dim, const WindowProperty flags)
    : flags(flags), dimensions(dim) {

#ifdef __EMSCRIPTEN__
    // force opengl es 3.0
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#else
    // macOS core profiel
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

    // Create the window
    sdl_window = SDL_CreateWindow(
        windowName,
        dim.w, dim.h,
        flags | SDL_WINDOW_OPENGL
    );
    if (!sdl_window)
        err::panic("Window::Window()", "Failed to make window with error: ",  SDL_GetError());
}

Window::~Window() {
    SDL_DestroyWindow(sdl_window);
}

void Window::_normalizeMousePos(float& x, float& y) const {
    int wx, wy;
    SDL_GetWindowSize(sdl_window, &wx, &wy);

    x = 2.f*(x-wx)/dimensions.w +1.f;
    y = -2.f*((y-wy)/dimensions.h) - 1.f;
}

void Window::setDimensions(const dim2 dim) const {
    SDL_SetWindowSize(sdl_window, dim.w, dim.h);
}

void Window::setWidth(uint32_t width) const {
    SDL_SetWindowSize(sdl_window, width, dimensions.h);
}

void Window::setHeight(uint32_t height) const {
    SDL_SetWindowSize(sdl_window, dimensions.w, height);
}

void Window::setPosition(const glm::vec2 pos) const {
    SDL_SetWindowPosition(sdl_window, pos.x, pos.y);
}

void Window::setFullscreen()  {
    flags |= WindowFullscreen;
    SDL_SetWindowFullscreen(sdl_window, true);
}

void Window::setWindowed() {
    flags &= ~WindowFullscreen;
    SDL_SetWindowFullscreen(sdl_window, false);
}

void Window::setResizable(const bool b) {
    if (b)  flags |= WindowResizable;
    else    flags &= ~WindowResizable;
    SDL_SetWindowResizable(sdl_window, b);
}

void Window::setFloatOnTop(const bool b) {
    if (b)  flags |= WindowFloatOnTop;
    else    flags &= ~WindowFloatOnTop;
    SDL_SetWindowAlwaysOnTop(sdl_window, b);
}

void Window::setMouseGrab(const bool b) {
    if (b)  flags |= WindowMouseConfined;
    else    flags &= ~WindowMouseConfined;
    SDL_SetWindowMouseGrab(sdl_window, b);
}

void Window::setMouseLocking(const bool hidden) {
    if (hidden)  flags |= WindowMouseHidden;
    else        flags &= ~WindowMouseHidden;
    SDL_SetWindowRelativeMouseMode(sdl_window, hidden);
}

void Window::setKeyboardGrab(const bool b) {
    if (b)  flags |= WindowKeyboardGrabbed;
    else    flags &= ~WindowKeyboardGrabbed;
    SDL_SetWindowMouseGrab(sdl_window, b);
}

void Window::setIcon(const char pathToImage[]) {
    SDL_Surface* surf; //= IMG_Load(pathToImage);
    #ifdef GAN_DEBUG
    if (!surf)
        std::cout << "Failed to load image: " << pathToImage << ".\n" << SDL_GetError() << std::endl;
    #endif
    SDL_SetWindowIcon(sdl_window, surf);
    SDL_DestroySurface(surf);
}

void Window::setName(const char name[]) const {
    SDL_SetWindowTitle(sdl_window, name);
}

void Window::setOpacity(const float opacity) const {
    if (flags & WindowTransparent)
        SDL_SetWindowOpacity(sdl_window, opacity);
    else
        err::panic("gan::Window::setOpacity()",
            "Cannot set window opacity, as flag 'WindowTransparent'"
            "was not enabled at launch.");
}

bool Window::isFullscreen() const {
    return flags & WindowFullscreen;
}

bool Window::isWindowed() const {
    return !(flags & WindowFullscreen);
}

bool Window::hasProperty(const WindowProperty flag) const {
    return flags & flag;
}

bool Window::isResizable() const {
    return flags & WindowResizable;
}

bool Window::isFloatOnTop() const {
    return flags & WindowFloatOnTop;
}

bool Window::isMouseConfined() const {
    return flags & WindowMouseConfined;
}

bool Window::isKeyboardGrab() const {
    return flags & WindowKeyboardGrabbed;
}

bool Window::isTransparent() const {
    return flags & WindowTransparent;
}

bool Window::isMouseLocked() const {
    return flags & WindowMouseHidden;
}

bool Window::isFocused() const {
    return flags & WindowFocused;
}

float Window::getOpacity() const noexcept {
    return SDL_GetWindowOpacity(sdl_window);
}

dim2 Window::getDimensions() const noexcept {
    return dimensions;
}

uint32_t Window::getWidth() const noexcept {
    return dimensions.w;
}

uint32_t Window::getHeight() const noexcept {
    return dimensions.h;
}

glm::vec2 Window::getPosition() const noexcept {
    int x, y;
    SDL_GetWindowPosition(sdl_window, &x, &y);
    return {static_cast<float>(x), static_cast<float>(y)};
}

SDL_Window* Window::_get_SDL_window() const {
    return sdl_window;
}

void Window::_onResize(SDL_Event& e) {
    const int w = e.window.data1;
    const int h = e.window.data2;
    glViewport(0, 0, w, h);
    dimensions = dim2{w, h};
}


