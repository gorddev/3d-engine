#pragma once

#define SDL_MAIN_USE_CALLBACKS 1
#define GLM_ENABLE_EXPERIMENTAL

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

// macros to consider for this file and what they do:

// #define OLAF_API › Tells the file to use the Book class instead
    // of the engine class, which is significantly easier to use
    // than the base Engine class. However, offers less flexibility.
    // Automatically called in OlafAPI.hpp.
// #define OLAF_APPSTATE_OBJ
    // Allows the user to define and pass around
    // an AppState object that holds user-defined information. Changes the signatures
    // of g_main(), g_iterate(), g_SDL_event(), and g_exit()
// #define OLAF_EXIT_FUNC
    // Lets the user define an exit function g_exit() such that they can
    // run code upon app exit.
// #define OLAF_SDL_EVENT_FUNC
    // Lets the user define an event function g_SDL_event() that
    // Passes the raw SDL event object to the function for more fine-grained
    // control. For those who #define OLAF_API, it's not necessary
    // with the included EventDispatcher, but it can be helpful for niche circumstances.

#ifndef OLAF_API //< If the OlafAPI is not defined, we use the standard engine.
#include "Engine.hpp"
namespace gan {
    using Book = Engine;
}
#else           //< Otherwise, we use the Book class: engine management much easier.
#include "beginner-api/Book.hpp"
#endif



// --------------------
// gan callback functions
// --------------------

/* The AppState object lets the user pass their own data throughout the program.
 * Its inclusion is optional and controlled via compiler directives.
* The user MUST implement the AppState object if they #define OLAF_APPSTATE_OBJ */
#ifdef OLAF_APPSTATE_OBJ
/// Determines whether the user wants to pass their own data through functions.
/// They must define their own @code AppState@endcode struct to pass data.
/// The @code g_main()@endcode and @code g_iterate()@endcode funcs will change signature
/// to expect a AppState as a return value and parameter.
struct AppState;
static AppState* userAppState;

/** The main function. Runs once upon initialization.
  * @param book The engine through with you interact with the world.
  * @param argc The number of arguments from the command line.
  * @param argv The character vector from the command line.
  * @return The @code AppState@endcode you want to pass around.
  * @warning The @code AppState@endcode _must_ be allocated with the @code new@endcode keyword. */
AppState* g_main(gan::Book& book, int argc = 0, char* argv[] = nullptr);
/** The iteration function. Called once every frame.
  * @param Book The engine used to interact with the world.
  * @param AppState The user-passed game-object.
  * @return 0 if you want to continue the program. -1 otherwise.*/
int g_iterate(gan::Book&, AppState*);
#ifdef OLAF_SDL_EVENT_FUNC
    /** Called on any given SDL_event */
    void g_SDL_event(gan::Book&, AppState*, const SDL_Event&);
#endif
#ifdef OLAF_EXIT_FUNC
    /** Called when the application wants to exit. Allows you to clean up resources
     *  and write to/delete files. */
    int g_exit(gan::Book&, AppState*);
#endif
#else //< if OLAF_APPSTATE_OBJ is not defined, callbacks pretend the struct AppState doesn't exist at all.
/** The main function. Runs one upon initialization.
  * @param book The engine through with you interact with the world.
  * @param argc The number of arguments from the command line.
  * @param argv The character vector from the command line.
  * @return @code 0@endcode if you want to continue with the program. Any other number if you want to quit. */
int g_main(gan::Book& book, int argc = 0, char* argv[] = nullptr);
/** The iteration function. Called once every frame.
  * @param book The engine used to interact with the world.
  * @return 0 if you want to continue the program. -1 otherwise. */
int g_iterate(gan::Book& book);
#ifdef OLAF_SDL_EVENT_FUNC
    /** Called on any given SDL_event */
    void g_SDL_event(gan::Book& book, const SDL_Event& event);
#endif
#ifdef OLAF_EXIT_FUNC
    /** Called when the application wants to exit. Allows you to clean up resources
     *  and write to/delete files. */
    int g_exit(gan::Book& book);
#endif
#endif


// ------------------------------------
// SDL Callback functions
// ------------------------------------


// SDL Initialization function. Where g_main() is called.
inline SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    // initialize SDL with just video.
    SDL_Init(SDL_INIT_VIDEO);
    // create our engine/book
    auto& engine = *new gan::Book("");
    // store it in the appstate variable so it's given to other functions.
    *appstate = &engine;

#ifdef OLAF_APPSTATE_OBJ //< If using AppState object, grab that from main.
    userAppState = g_main(engine, argc, argv);
    if (!userAppState) {
        gan::print("Ending program now. Received a null pointer for the `AppState`: ", userAppState, ".\n");
        return SDL_APP_SUCCESS;
    }
#else   //< Otherwise, grab the error code.
    int status = g_main(engine, argc, argv);
    if (status != 0) {
        gan::print("Ending program. User returned from g_main() with exit code ",
            status, ".\nTo continue with program, ensure that you return 0.\n");
        return SDL_APP_SUCCESS;
    }
#endif
    ///////////////////////

    return SDL_APP_CONTINUE;
}

static gan::Camera cam;

// SDL App Iteration function. Where g_iterate() is called.
inline SDL_AppResult SDL_AppIterate(void *appstate) {
    // re-cast the engine.
    auto& engine = *static_cast<gan::Book*>(appstate);

    // updates timer and keyboard state.
    engine.iterate();
    // clears the renderer.
    engine.render.clear(cam);

    GL_CHECK();

#ifdef OLAF_APPSTATE_OBJ    //< If AppState object, call iterate with the userAppState
    g_iterate(engine, userAppState);
#else                       //< Otherwise, call it without the AppState object.
    g_iterate(engine);
#endif

    // present the current render
    engine.render.present();

    /* clear the current scroll wheel position of the mouse, because wheels are handled by the event system
       and not per-frame updates like cursor position & keyboard state. */
    #ifdef OLAF_API
        gan::InputUpdater::clearMouseWheelPosition(engine.mouse);
    #else   //< If we aren't using the beginner API, the mouse is stored in the EngineCore.
        gan::InputUpdater::clearMouseWheelPosition(engine.core.mouse);
    #endif

    return SDL_APP_CONTINUE;
}

// Handle events
inline SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    // static case down to our book. (will be either an Engine or a Book, depending on if the #define OLAF_API was called)
    auto& book = *static_cast<gan::Book*>(appstate);

    switch (event->type) {
    // When SDL want to quit the application.
    case SDL_EVENT_QUIT:
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
        return SDL_APP_SUCCESS;
    // On window resizing events
    case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
    case SDL_EVENT_WINDOW_RESIZED:
    case SDL_EVENT_WINDOW_DISPLAY_CHANGED:
    case SDL_EVENT_WINDOW_MINIMIZED:
        #ifdef OLAF_API
            book.window._onResize(*event);
        #else   //< If not using the beginner API, the window is stored in the EngineCore.
            book.core.window.onResize(*event);
        #endif
        break;
    // On mouse wheel movements
    case SDL_EVENT_MOUSE_WHEEL:
        #ifdef OLAF_API
            gan::InputUpdater::updateMouseWheelWithSDLEvent(*event, book.mouse);
        #else   //< If not using the beginner API, the mouse is stored in the EngineCore.
            gan::InputUpdater::updateMouseWheelWithSDLEvent(*event, book.core.mouse);
        #endif
    default:
        break;
    }

    #ifdef OLAF_SDL_EVENT_FUNC //< If the user opted in to receiving SDL_Events.
        #ifdef OLAF_APPSTATE_OBJ
            g_SDL_event(book, userAppState, *event);
        #else
            g_SDL_event(book, *event);
        #endif
    #endif

    #ifdef OLAF_API //< If the user is using the beginner API, they get access to the EventDispatcher.
        book.events.dispatchEvent(*event);
    #endif

    return SDL_APP_CONTINUE;
}

// Cleanup
inline void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    #ifdef OLAF_EXIT_FUNC   //< If the user opted in to having the exit function called.
    auto& book = *static_cast<gan::Book*>(appstate);
        #ifdef OLAF_APPSTATE_OBJ
            g_exit(book, userAppState);
        #else
            g_exit(book);
        #endif
    #endif
    delete static_cast<gan::Book*>(appstate);
    SDL_Quit();
}



