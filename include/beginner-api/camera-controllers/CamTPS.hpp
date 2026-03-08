#pragma once

#include <core/EngineCore.hpp>

#include "CamController.hpp"

/* Created by Gordie Novak on 3/3/26.
 * Purpose: 
 * Interface for a third-person camera perspective.
 * Automatically transitions to first person when camera
 * is on the axis of rotation point */


namespace gan {


    /// Third Person Camera Controller.
    struct CamTPS final : CamController {
    private:
        Camera& cam;
    public:

        /** Requires a camera to initialize */
        explicit CamTPS(Camera& cam, float startingYaw = lowp_pi, float startingPitch = 0.1f)
            : cam(cam), yaw(startingYaw), pitch(startingPitch) {}

        /** Automatically applies a keyboard camera control scheme with
        * the following default controls. Update controls with the @code keybinds@endcode array.
        * - @code W@endcode Move in the @code -z@endcode direction.
        * - @code S@endcode Move in the @code +z@endcode direction.
        * - @code A@endcode Move in the @code -x@endcode direction.
        * - @code D@endcode Move in the @code +x@endcode direction.
        * - @code Shift@endcode Move in the @code -y@endcode direction.
        * - @code Space@endcode Move in the @code +y@endcode direction. */
        void useKeyboard(
            const KeyboardState& keyboard,
            const Clock& clock
        ) override {
            const float t_speed = keyboardSpeed * static_cast<float>(clock.dt) * 0.3f;

            // Handle camera position movement
            if (keyboard.isHeld(keybinds[0])) {
                pitch += t_speed;
            } if (keyboard.isHeld(keybinds[1])) {
                pitch -= t_speed;
            } if (keyboard.isHeld(keybinds[2])) {
                yaw -= t_speed;
            } if (keyboard.isHeld(keybinds[3])) {
                yaw += t_speed;
            } if (keyboard.isHeld(keybinds[4])) {
                cam.moveForward(t_speed*5.f);
            } if (keyboard.isHeld(keybinds[5])) {
                cam.moveForward(-t_speed*5.f);
            }
        }

        /** Moves the camera via the mouse, emulating a third-person perspective. Change the following variables
          * to alter camera behavior:
            - @code this.sensitivity@endcode › How much mouse movements alter direction.
            - @code this.min_pitch/max_pitch@endcode › How far up and down the camera is able to look (in radians) */
        void useMouse(
            const MouseState& mouse,
            const Clock&
        ) override {
            // Handle camera rotation movement.

            if (!requireRightClick || mouse.isRightClicked()) {
                yaw -= mouse.getDeltaMouseX() * mouseSensitivity;
                pitch += mouse.getDeltaMouseY() * mouseSensitivity;
            }
            // prevent the pitch from moving below ground or above sky
            pitch = std::clamp(pitch, pitch_min, pitch_max);

            cam.moveForward(mouse.getScrollWheelY()*mouseSensitivity*15.f);

            const auto diff = glm::distance(cam.pos, axis);
            cam.pos = axis + diff *
                glm::normalize(glm::vec3{sinf(yaw), sinf(pitch), cosf(yaw)});
            cam.forward = glm::normalize(-cam.pos + axis);
        }

        // VARIABLES BELOW

        // <><><> Mouse Control Values <><><>
        /** Rotation along the x-z plane.
          * - @code yaw = 0.f@endcode › points along x-axis.
          * - @code yaw = pi/2@endcode › points along z-axis. */
        float yaw = lowp_pi;
        /** Rotation along the y-axis. Automatically clamped to the variables @code pitch_min@endcode
          * and @code pitch_max@endcode.
          * - @code pitch = 0.f@endcode › level
          * - @code pitch = pi/2@endcode › towards the sky
          * - @code pitch = -pi/2@endcode › towards the ground */
        float pitch = 0.f;
        float pitch_min = -lowp_pi/2, pitch_max = lowp_pi/2;


        float mouseSensitivity = 0.01f;     ///< Sensitivity of mouse movement.
        glm::vec3 axis{0.f,0.f,0.f};  ///< Axis of movement in 3rd person mode.
        bool scrollToZoom = true;           ///< If the user can scroll the mouse and zoom in/out in third person.
        bool requireRightClick = true;      ///< Forces the user to right-click to change looking direction.

        // *************************************
        // <><><> Keyboard control values <><><>
        // *************************************
        /** Keybinds for keyboard movement. Uses the @code KeypressType@endcode enum.
          * Feel free to remap as desired.
          * - @code [0]@endcode: Forward
          * - @code [1]@endcode: Backwards
          * - @code [2]@endcode: Left
          * - @code [3]@endcode: Right
          * - @code [4]@endcode: Up
          * - @code [5]@endcode: Down */
        std::array<KeypressType, 6> keybinds
            = {KEY_W, KEY_S, KEY_A, KEY_D, KEY_SPACE, KEY_LSHIFT};

        float keyboardSpeed = 0.01f;               ///< Speed of the camera.
    };

}

