#include <Engine.hpp>
// Created by Gordie Novak on 2/28/26.
//

using namespace gan;

Engine::Engine(const char windowName[], uint32_t windowW, uint32_t windowH, WindowProperty flags)
    : core(windowName, {windowW, windowH}, flags), render(core) {
    core.init();
}

void Engine::iterate() {
    core.iterate();
    render.clear(core.camera);
}

