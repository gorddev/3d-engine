#pragma once

#include <Engine.hpp>
#include <beginner-api/factory/ShapeFactory.hpp>

#include "events/EventDispatcher.hpp"
// Created by Gordie Novak on 2/27/26.
// The book is an object that makes the Engine API
// much friendlier to handle for beginners. It's less flexible though
// and less efficient. You also can't std::move it.

namespace gan {
    /// Book is a convenient API for handling the engine.
    class Book {
    private:
        Engine engine;

        friend SDL_AppResult (::SDL_AppInit(void **appstate, int argc, char *argv[]));
        friend SDL_AppResult (::SDL_AppIterate(void *appstate));

        /** Constructor for book. All it does is fill out the reference slots. Private so
          * users don't accidentally make another one. */
        explicit Book(const char windowName[] = "Olaf API Application",
            uint32_t windowWidth = 600, uint32_t windowHeight = 500,
            const WindowProperty flags = WindowResizable)
            : engine{windowName, windowWidth, windowHeight, flags},
            keyboard(engine.core.keyboard),
            mouse(engine.core.mouse),
            render(engine.render),
            shaders(engine.render.shaders),
            shapes(engine.render.shaders),
            clock(engine.core.clock),
            window(engine.core.window),
            camera(engine.core.camera) {}

        /// Iterates the book forward by one frame & updates internal values.
        void iterate() {
            engine.iterate();
        }

    public:
        EventDispatcher events; ///< Allows the user to add listeners for specific events.
        KeyboardState& keyboard;    ///< API for the current keyboard state.
        MouseState& mouse;          ///< API for the current mouse state.
        Renderer& render;           ///< Renderer you can use to draw objects.
        ShaderHandler& shaders;     ///< API for changing/swapping/writing shaders.
        ShapeFactory shapes;        ///< API for creating shapes
        Clock& clock;               ///< API for querying for current time
        Window& window;             ///< API for handling the window
        union {Camera camera, cam;};///< API for camera.

        /// Automatic conversion to an EngineCore for easier interaction with non-api functions.
        // ReSharper disable once CppNonExplicitConversionOperator
        operator EngineCore&() { // NOLINT(*-explicit-constructor)
            return engine.core;
        }

        Book operator=(const Book& book)    = delete; /// Nope, you can't do that
        Book(const Book& book)              = delete; /// Nope, you can't do that
        Book operator=(Book&& book)         = delete; /// Nope, you can't do that
        Book(const Book&& book)             = delete; /// Nope, you can't do that

    };
}