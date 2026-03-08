#pragma once

#include <glm/vec2.hpp>
#include <SDL3/SDL_mouse.h>

// made by gordie novak feb 25th

// used to store all the input data: read only except for the InputReceiver.



namespace gan {
    class Engine;

    enum MouseButtonType {
        MOUSE_L = SDL_BUTTON_LMASK,
        MOUSE_R = SDL_BUTTON_RMASK,
        MOUSE_MIDDLE = SDL_BUTTON_MMASK,
        MOUSE_X1 = SDL_BUTTON_X1MASK,
        MOUSE_X2 = SDL_BUTTON_X2MASK,
    };
    
    class MouseState {
        friend struct InputUpdater;
        friend class Window;
        /// Position & delta position of the mouse.
        glm::vec2 mousePos, deltaMousePos;
        /// How much the scroll wheel has movel
        glm::vec2 deltaScrollWheel = glm::vec2(0.f);
        /// Bitmask representing the mouse state.
        uint32_t mouseFlags;

        friend class EngineCore;

        MouseState() = default;
    public:
        // <><><> Mouse positioning <><><>
        /// Returns the current mouse position.
        [[nodiscard]] glm::vec2 getMousePos() const {
            return mousePos;
        }
        /// Returns the x-position of the mouse
        [[nodiscard]] float getMouseX() const {
            return mousePos.x;
        }
        /// Returns the y-position of the mouse
        [[nodiscard]] float getMouseY() const {
            return mousePos.y;
        }
        /// Returns the change in mouse position
        [[nodiscard]] glm::vec2 getDeltaMousePos() const {
            return deltaMousePos;
        }

        /// Returns the change in MousePosX()
        [[nodiscard]] float getDeltaMouseX() const {
            return deltaMousePos.x;
        }

        /// Returns the change in MousePosY()
        [[nodiscard]] float getDeltaMouseY() const {
            return deltaMousePos.y;
        }

        /// Returns the total distance the scroll
        [[nodiscard]] glm::vec2 getScrollWheelDist() const {
            return deltaScrollWheel;
        }

        /// Returns the change in the scroll wheel x
        [[nodiscard]] float getScrollWheelX() const {
            return deltaScrollWheel.x;
        }

        /// Returns the change in the scroll wheel y
        [[nodiscard]] float getScrollWheelY() const {
            return deltaScrollWheel.y;
        }

        /// @param button The type of mouse action you want to verify.
        /// - @code MOUSE_L@endcode › Left click
        /// - @code MOUSE_R@endcode › Right click
        /// - @code MOUSE_MIDDLE@endcode › Middle click
        /// - @code MOUSE_X1@endcode › First side button of the mouse
        /// - @code MOUSE_X2@endcode › Second side button of the mouse.
        /// @return @code true@endcode if the provided option was clicked this frame.
        [[nodiscard]] bool mouseState(const MouseButtonType button) const {
            return mouseFlags & button;
        }

        /// @return @code true@endcode if the mouse was left-clicked this frame.
        [[nodiscard]] bool isLeftClicked() const {
            return mouseFlags & MOUSE_L;
        }

        /// @return @code true@endcode if the mouse was right-clicked this frame
        [[nodiscard]] bool isRightClicked() const {
            return mouseFlags & MOUSE_R;
        }

        /// @return @code true@endcode if the mouse was middle-clicked this frame
        [[nodiscard]] bool isMiddleClicked() const {
            return mouseFlags & MOUSE_MIDDLE;
        }

        /// @return @code true@endcode if the mouse's first side button was clicked
        [[nodiscard]] bool isX1Clicked() const {
            return mouseFlags & MOUSE_X1;
        }

        /// @return @code true@endcode if the mouse's second side button was clicked
        [[nodiscard]] bool isX2Clicked() const {
            return mouseFlags & MOUSE_X2;
        }

        // Make sure the mouse state can't be accidentally copied.
        MouseState operator=(MouseState&&)              = delete;   ///< Nope. You can't do that.
        MouseState operator=(const MouseState& other)   = delete;   ///< Nope. You can't do that.
        MouseState(MouseState&& other)                  = delete;   ///< Nope. You can't do that.
        MouseState(const MouseState& other)             = delete;   ///< Nope. You can't do that.
    };



}
