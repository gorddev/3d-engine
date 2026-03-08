#pragma once

#include <functional>
#include <SDL3/SDL_events.h>
#include <types/custom-data-structures/map_vector.hpp>

// Created by Gordie Novak on 2/28/26.
// created to allow for event dispatching relatively easily
// Accepts both event listeners and just plain functions or lambdas.
// Functions must be of the form {void myFunc(const SDL_Event& e)};

namespace gan {

    enum EventType : uint32_t{
        EVENT_KEY_DOWN              = SDL_EVENT_KEY_DOWN,
        EVENT_KEY_UP                = SDL_EVENT_KEY_UP,
        EVENT_MOUSE_CLICK           = SDL_EVENT_MOUSE_BUTTON_DOWN,
        EVENT_MOUSE_RELEASE         = SDL_EVENT_MOUSE_BUTTON_UP,
        EVENT_MOUSE_MOTION          = SDL_EVENT_MOUSE_MOTION,
        EVENT_MOUSE_WHEEL           = SDL_EVENT_MOUSE_WHEEL,
        EVENT_WINDOW_MOVED          = SDL_EVENT_WINDOW_MOVED,
        EVENT_WINDOW_RESIZED        = SDL_EVENT_WINDOW_RESIZED,
        EVENT_WINDOW_MOUSE_ENTER    = SDL_EVENT_WINDOW_MOUSE_ENTER,
        EVENT_WINDOW_MOUSE_LEAVE    = SDL_EVENT_WINDOW_MOUSE_LEAVE,
        EVENT_WINDOW_FOCUSED        = SDL_EVENT_WINDOW_FOCUS_GAINED,
        EVENT_WINDOW_UNFOCUSED      = SDL_EVENT_WINDOW_FOCUS_LOST,
    };

    struct EventListener {
        virtual ~EventListener() = default;

        /** @return @code true@endcode if you want to consume the event */
        virtual bool onEvent(const SDL_Event& event) = 0;
    };

    class EventDispatcher {
        using ListenerFunc = std::function<void(const SDL_Event&)>;
        struct EventListenerFunc;
    private:
        /** Links event types to a vector of EventHandlers. */
        std::unordered_map<EventType, std::vector<EventListener*>> events;
        std::vector<EventListenerFunc*> function_listeners;
    public:
        // Add listeners via inheritance and class.
        void addListener(EventType type, EventListener* listener);
        void addListener(std::initializer_list<EventType> type, EventListener* listener);

        /** Removes a given listener from the map vector
         * @return @code true@endcode if the removal was successful. */
        bool removeListener(EventType type, EventListener* listener);
        /** Removes all instances of the listener. Very expensive to call. **/
        void removeListener(EventListener* listener);


        // Add listeners via function rather than via class.
        void addListener(EventType type, ListenerFunc listener);
        void addListener(std::initializer_list<EventType> types, ListenerFunc listener);

        // Remove listeners via function rather than class.
        void removeListener(EventType type, ListenerFunc listener);
        void removeListener(ListenerFunc listener);

        // Dispatches the given events to all listeners
        void dispatchEvent(const SDL_Event& event);

        ~EventDispatcher();

    private:
        struct EventListenerFunc final : EventListener {
            ListenerFunc listener;

            explicit EventListenerFunc(const ListenerFunc listener) noexcept
                : listener(listener) {}

            bool onEvent(const SDL_Event& event) override {
                listener(event);
                return true;
            }
        };
    };

}