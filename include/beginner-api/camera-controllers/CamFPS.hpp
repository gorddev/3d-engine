#pragma once

#include <core/EngineCore.hpp>

#include "CamController.hpp"
#include "beginner-api/olaf-utilities/CameraController.hpp"

/* Created by Gordie Novak on 3/3/26.
 * Purpose: 
 * This is a camera controller that works for
 * first person points of view.*/



namespace gan {


    struct CamFPS final : CamController {

        /** Requires a camera to initialize */
        explicit CamFPS(
            Camera& cam,
            const float startingYaw = lowp_pi,
            const float startingPitch = 0.1f
        )
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

            const float t_speed = keyboardSpeed * static_cast<float>(clock.dt);
            // Handle camera position movement
            if (keyboard.isHeld(keybinds[0])) {
                cam.moveForward(t_speed);
            } if (keyboard.isHeld(keybinds[1])) {
                cam.moveForward(-t_speed);
            } if (keyboard.isHeld(keybinds[2])) {
                cam.moveHorizontal(-t_speed);
            } if (keyboard.isHeld(keybinds[3])) {
                cam.moveHorizontal(t_speed);
            } if (keyboard.isHeld(keybinds[4])) {
                cam.moveVertical(t_speed);
            } if (keyboard.isHeld(keybinds[5])) {
                cam.moveVertical(-t_speed);
            }
        }

        /** Moves the camera via the mouse, emulating a first-person perspective. Change the following variables
          * to alter camera behavior:
            - @code this.mouseSensitivity@endcode › How much mouse movements alter direction.
            - @code this.pitchMin/pitchMax@endcode › How far up and down the camera is able to look (in radians) */
        void useMouse(
            const MouseState& mouse,
            const Clock&
        ) override {
            // Handle camera rotation movement.
            if (!requireRightClick || mouse.isRightClicked()) {
                yaw -= mouse.getDeltaX() * mouseSensitivity * 0.01f;
                pitch -= mouse.getDeltaY() * mouseSensitivity * 0.01f;
                pitch = std::clamp(pitch, pitchMin, pitchMax);

                cam.forward = {sinf(yaw), sinf(pitch), cosf(yaw)};
                cam.up.y = cosf(pitch);
            }
        }

        // VARIABLES BELOW
        // Reference to the camera this Controller manages.
        Camera& cam;

        /* Mouse Control Values */
        /** Rotation along the x-z plane.
          * - @code yaw = 0.f@endcode › points along x-axis.
          * - @code yaw = pi/2@endcode › points along z-axis. */
        float yaw = lowp_pi;
        /** Rotation along the y-axis. Automatically clamped to the variables @code pitchMin@endcode
          * and @code pitchMax@endcode.
          * - @code pitch = 0.f@endcode › level
          * - @code pitch = pi/2@endcode › towards the sky
          * - @code pitch = -pi/2@endcode › towards the ground */
        float pitch = 0.f;

        /* */
        float pitchMin = -lowp_pi/2.f;     ///< How low the camera can tilt
        float pitchMax =  lowp_pi/2.f;     ///< How high the camera can tilt (in radians)


        float mouseSensitivity  = 1.f;     ///< Sensitivity of mouse movement.
        bool  scrollToZoom      = true;    ///< If the user can scroll the mouse and zoom in/out in third person.
        bool  requireRightClick = true;    ///< Forces the user to right-click to change looking direction.

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

        float keyboardSpeed = 0.01f;       ///< Speed of the camera.

    };

}
