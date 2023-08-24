//
// Created by Flo on 17/01/2023.
//

#pragma once

#include "SFML/Graphics.hpp"
#include "Observable.hpp"
#include <array>
#include <functional>

namespace KrabEngine
{
    class WindowEvents
    {
    public:
        static void ProcessEvents(sf::RenderWindow& renderWindow);

        [[nodiscard]] static bool GetEvent(sf::Event::EventType eventType);
        [[nodiscard]] static bool GetEvent(sf::Event::EventType eventType, sf::Event& outEvent);

        static SignalSystem::Observable<sf::Event> lastEvent;
        static std::string UnlockAfterEvent;

    private:
        static void clearEvents();

    private:
        static std::array<sf::Event, sf::Event::Count> s_events;
    };

} // Engine
