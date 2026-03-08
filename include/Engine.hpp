#pragma once

#include <rendering/Renderer.hpp>
#include <rendering/Window.hpp>
#include <core/EngineCore.hpp>

namespace gan {

    class Engine final {
    public:
        EngineCore core;
        Renderer render;

        Engine(const char windowName[], uint32_t windowW, uint32_t windowH, WindowProperty flags);

        void iterate();

        // Engine cannot be moved or everything tends to break with references.
        Engine(Engine&&) = delete;
        Engine& operator=(Engine&&) = delete;
    };
}
