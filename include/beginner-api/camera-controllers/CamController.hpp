#pragma once
#include "core/EngineCore.hpp"

/* Created by Gordie Novak on 3/3/26.
 * Purpose: 
 * Provides a consistent interface for all camera controllers.
 * Contains no member variables. */

namespace gan {


    /** Public interface CameraControllers should implement to ensure consistent usage, naming,
      * and polymorphism. Must implement the following pure virtual functions
      * - @code virtual void useKeyboard(Camera&, const EngineCore&) = 0@endcode › Updates camera with the keyboard.
      * - @code virtual void useMouse(Camera&, const EngineCore&) = 0@endcode › Updates camera with the mouse.
      * - @code virtual void apply(Camera&, const EngineCore&) {}@endcode › Non-virtual optional function. */
    struct CamController {

        static constexpr float lowp_pi = 3.1415; ///< Stands for "low precision pi"

        // Pure virtual function for keyboard use.
        virtual void useKeyboard(const KeyboardState&, const Clock&) = 0;

        // Pure virtual function for mouse use.
        virtual void useMouse(const MouseState&, const Clock&) = 0;

        // Virtual function for applying camera parameters
        virtual void apply(const Clock&) {}

        // Virtual destructor
        virtual ~CamController() = default;
    };

}

