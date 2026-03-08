//
// Created by Gordie Novak on 2/25/26.
// Stores all the most important and relevant information for the engine!

#pragma once
#include "scene/Camera.hpp"
#include "input/InputUpdater.hpp"
#include "input/KeyboardState.hpp"
#include "input/MouseState.hpp"
#include "rendering/Window.hpp"
#include "rendering/textures/TextureRegister.hpp"
#include "time/Clock.hpp"

namespace gan {

    /// The @code EngineCore@endcode is the primary set of independent APIs
    /// the Engine uses to distribute and disseminate information.
    class EngineCore final {
    public:
        Camera camera;              ///< Camera that influences how things are rendered.
        KeyboardState keyboard;     ///< Contains the current state of the keyboard.
        MouseState mouse;           ///< Contains the current state of the keyboard.
        TextureRegister texRg;      ///< Stores & verifies all submitted textures.
        Clock clock;                 ///< Keeps track of total time, frames, delta time, ect.
        Window window;              ///< Window the user is viewing everything through.
    private:
        friend class Engine;

        EngineCore(const char windowName[], const dim2 windowSize, const WindowProperty flags)
            : mouse(), clock(TimeUpdater::createTimerFromSDL()), window(windowName, windowSize, flags) {
        }

        void init() {
            texRg.init();
        }

        void iterate() {
            TimeUpdater::updateTimerWithSDL(clock);
            InputUpdater::updateKeyboardWithSDL(keyboard);
            InputUpdater::updateMouseWithSDL(mouse);
        }
    };

}
