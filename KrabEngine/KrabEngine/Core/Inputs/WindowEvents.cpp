//
// Created by Flo on 17/01/2023.
//

#include "WindowEvents.hpp"
#include "InputSignal.hpp"

#include <iostream>

#include "Observable.hpp"

namespace KrabEngine
{
    std::array<sf::Event, sf::Event::Count> WindowEvents::s_events;
    SignalSystem::Observable<sf::Event> WindowEvents::lastEvent;
    std::string WindowEvents::UnlockAfterEvent = "";

    void WindowEvents::ProcessEvents(sf::RenderWindow &renderWindow)
    {
        clearEvents();

        sf::Event event {};

        while(renderWindow.pollEvent(event))
        {
#ifdef IMGUI_SUPPORT
            // ImGui Events Handling
            //ImGui::SFML::ProcessEvent(event);
#endif
            s_events[event.type] = event;

            lastEvent = event;

            auto mouse = SignalSystem::InputConfig::Get()->GetMouseBindingName(event.type, event.mouseButton.button);
            if (!mouse.empty())
                for(const auto& name : mouse)
                    SignalSystem::InputSignal::Get()->Emit(name);

            auto key = SignalSystem::InputConfig::Get()->GetKeyBindingName(event.type, event.key.code);
            if (!key.empty())
                for(const auto& name : key)
                    SignalSystem::InputSignal::Get()->Emit(name);

            auto eventName = SignalSystem::InputConfig::Get()->GetEventBindingName(event.type);
            if (!eventName.empty())
                SignalSystem::InputSignal::Get()->Emit(eventName);

            if(SignalSystem::InputSignal::Get()->IsLock())
                if(std::find(key.begin(), key.end(), UnlockAfterEvent) != key.end()
                || std::find(mouse.begin(), mouse.end(), UnlockAfterEvent) != mouse.end()
                || eventName == UnlockAfterEvent)
                {
                    SignalSystem::InputSignal::Get()->Unlock();
                    UnlockAfterEvent = "";
                }
        }
    }

    bool WindowEvents::GetEvent(sf::Event::EventType eventType)
    {
        return (s_events[eventType].type != sf::Event::Count);
    }

    bool WindowEvents::GetEvent(sf::Event::EventType eventType, sf::Event &outEvent)
    {
        if (s_events[eventType].type != sf::Event::Count)
        {
            outEvent = s_events[eventType];
            return true;
        }

        return false;
    }

    void WindowEvents::clearEvents()
    {
        for (auto& event : s_events)
        {
            event.type = sf::Event::Count;
        }
    }
} // KrabEngine