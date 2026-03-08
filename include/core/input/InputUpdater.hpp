#pragma once
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_events.h>

#include "KeyboardState.hpp"
#include "MouseState.hpp"

// made by gordie novak feb 25th. handles receiving and managing input, as
// well as callbacks for various user functions.

namespace gan {


    struct InputUpdater {

        /// Gets the current keyboard and mouse state from SDL and updates the given InputState object with it.
        /// @param mState The @code InputState@endcode object to update with SDL information.
        static void updateMouseWithSDL(MouseState& mState) {
            // Now get the current mouse positions!
            SDL_GetRelativeMouseState(&mState.deltaMousePos.x, &mState.deltaMousePos.y);
            // get all the juicy mouse flags.
            mState.mouseFlags = SDL_GetMouseState(&mState.mousePos.x, &mState.mousePos.y);
            // that's it
        }

        /// @warning Function assumes that the provided SDL_Event& e is a EVENT_MOUSEWHEEL
        static void updateMouseWheelWithSDLEvent(const SDL_Event& e, MouseState& mState) {
            mState.deltaScrollWheel = {e.wheel.x, e.wheel.y};
        }

        static void clearMouseWheelPosition(MouseState& mState) {
            mState.deltaScrollWheel = {0, 0};
        }

        static void updateKeyboardWithSDL(KeyboardState& kState) {
            // Gets the active keyboard state
            const auto ptr = SDL_GetKeyboardState(nullptr);
            // Memcpy the entire thing into our input state object.
            std::memcpy(kState.keys.data(), ptr, KeyboardState::maxKeys);
        }

    };

}