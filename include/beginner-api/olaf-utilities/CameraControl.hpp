#pragma once
#include "core/input/KeyboardState.hpp"
#include "core/input/MouseState.hpp"
#include "../../scene/Camera.hpp"

/* Created by Gordie Novak on 3/1/26.
 * Purpose: 
 * Provides easy Camera control consolidated into a function.
 * This way, camera control is easily enabled/disabled. */

namespace gan {
    /** Automatically applies a keyboard camera control scheme with
      * the following default controls:
      * - @code W@endcode Move in the @code -z@endcode direction.
      * - @code S@endcode Move in the @code +z@endcode direction.
      * - @code A@endcode Move in the @code -x@endcode direction.
      * - @code D@endcode Move in the @code +x@endcode direction.
      * - @code Shift@endcode Move in the @code -y@endcode direction.
      * - @code Space@endcode Move in the @code +y@endcode direction.
      * @param cam Camera you want controlled
      * @param keyboard The keyboard interface
      * @param speed Speed you want the camera to move at.
      * @param keybinds Optional › Lets you set custom keybinds for the camera.
      * Must be ordered: @code {FORWARD, BACK, LEFT, RIGHT, UP, DOWN}@endcode
     */
    inline void cameraKeyboardControl(
        Camera& cam,
        const KeyboardState& keyboard,
        const float speed = 0.1f,
        const std::array<KeypressType, 6>& keybinds =
            {KEY_W, KEY_S, KEY_A, KEY_D, KEY_SPACE, KEY_LSHIFT}
    ) {
        // Handle camera position movement
        if (keyboard.isHeld(keybinds[0])) {
            cam.moveForward(speed);
        } if (keyboard.isHeld(keybinds[1])) {
            cam.moveForward(-speed);
        } if (keyboard.isHeld(keybinds[2])) {
            cam.moveHorizontal(-speed);
        } if (keyboard.isHeld(keybinds[3])) {
            cam.moveHorizontal(speed);
        } if (keyboard.isHeld(keybinds[4])) {
            cam.moveVertical(speed);
        } if (keyboard.isHeld(keybinds[5])) {
            cam.moveVertical(-speed);
        }
    }

    /// Moves the camera in tandem with the mouse
    /// @param cam The camera you want to control with the mouse
    /// @param mouse The @code MouseState@endcode object that holds mouse information.
    /// @param sensitivity How much mouse movement influences camera rotation.
    /// @param requireRightClick Requires that right click be held to rotate the camera.
    inline void cameraMouseControl(
        Camera& cam,
        const MouseState& mouse,
        const float sensitivity = 0.01f,
        bool requireRightClick = true
    ) {
        // Handle camera rotation movement.
        if (!requireRightClick || mouse.isRightClicked()) {
            constexpr float pi = 3.14159;
            static float yaw = pi, pitch = 0.f;
            yaw -= mouse.getDeltaMouseX() * sensitivity;
            pitch -= mouse.getDeltaMouseY() * sensitivity;
            pitch = std::clamp(pitch, -pi/2.f, pi/2.f);

            cam.forward = {sinf(yaw), sinf(pitch), cosf(yaw)};
            cam.up.y = cosf(pitch);
        }
    }

    /// Moves the camera in tandem with the mouse. Contains scroll wheel support. For
    /// zooming in and zooming out.
    /// @param cam The camera you want to control with the mouse
    /// @param mouse The @code MouseState@endcode object that holds mouse information.
    /// @param sensitivity How much mouse movement influences camera rotation.
    /// @param requireRightClick Requires that right click be held to rotate the camera.
    inline void cameraMouseControl3rdPerson(
        Camera& cam,
        const MouseState& mouse,
        const glm::vec3 axis,
        float sensitivity = 1.f,
        bool requireRightClick = true
    ) {
        sensitivity *= 0.01f;
        constexpr float pi = 3.14159;
        static float yaw = pi, pitch = 0.1f;
        // Handle camera rotation movement.
        if (!requireRightClick || mouse.isRightClicked()) {
            yaw -= mouse.getDeltaMouseX() * sensitivity;
            pitch += mouse.getDeltaMouseY() * sensitivity;
        }
        cam.moveForward(mouse.getScrollWheelY() * sensitivity*35.f);

        pitch = std::clamp(pitch, -pi/2.f, pi/2.f);

        auto diff = glm::distance(cam.pos, axis);
        cam.pos = axis + diff * glm::normalize(glm::vec3{sinf(yaw), sinf(pitch), cosf(yaw)});
        cam.forward = glm::normalize(-cam.pos + axis);
        cam.up.y = cosf(pitch);

    }

}
