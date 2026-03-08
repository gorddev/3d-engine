#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <ostream>
#include <ranges>
#include <beginner-api/events/EventDispatcher.hpp>
#include <glm/gtx/type_trait.hpp>

using namespace gan;

void EventDispatcher::addListener
    (EventType type, EventListener* listener)
{
    events[type].push_back(listener);
}

void EventDispatcher::addListener
    (const std::initializer_list<EventType> types, EventListener *listener)
{
    for (auto& type: types) {
        events[type].push_back(listener);
    }
}

void EventDispatcher::addListener(EventType type, ListenerFunc listener) {
    auto* wrap = new EventListenerFunc(listener);
    events[type].push_back(wrap);
    function_listeners.push_back(wrap);
}

void EventDispatcher::addListener(std::initializer_list<EventType> types, ListenerFunc listener) {
    auto* wrap = new EventListenerFunc(listener);
    for (auto& type : types) {
        events[type].push_back(wrap);
    }
    function_listeners.push_back(wrap);
}

void EventDispatcher::removeListener(EventType type, ListenerFunc listener) {
    EventListener* list = nullptr;
    for (const auto& f : function_listeners) {
        if (f->listener.target<void (const SDL_Event& e)>()
            == listener.target<void (const SDL_Event& e)>() )
        {
            list = f;
        }
    }
    if (list == nullptr)
        return;
    removeListener(type, list);
}

void EventDispatcher::removeListener(ListenerFunc listener) {
    EventListener* list = nullptr;
    for (const auto& f : function_listeners) {
        if (f->listener.target<void (const SDL_Event& e)>()
            == listener.target<void (const SDL_Event& e)>() )
        {
            list = f;
        }
    }
    if (list == nullptr)
        return;
    removeListener(list);
}

void EventDispatcher::dispatchEvent(const SDL_Event &event) {
    if (events.contains(static_cast<EventType>(event.type))) {
        std::cerr << "detected matching event" << std::endl;
        for (const auto& e : events[static_cast<EventType>(event.type)]) {
            std::cerr << "calling on event" << std::endl;
            e->onEvent(event);
        }
    }
}

bool EventDispatcher::removeListener(EventType type, EventListener *listener) {
    auto& eventVec = events[type];
    auto it = std::ranges::find(eventVec, listener);
    if (it != eventVec.end()) {
        eventVec.erase(it);
        return true;
    }
    return false;
}

void EventDispatcher::removeListener(EventListener *listener) {
    for (auto &vec: events | std::views::values) {
        auto it
            = std::ranges::find(vec, listener);
        while (it != vec.end()) {
            vec.erase(it);
            it = std::ranges::find(vec, listener);
        }
    }
}

EventDispatcher::~EventDispatcher() {
    for (const auto& e : function_listeners)
        delete e;
}


