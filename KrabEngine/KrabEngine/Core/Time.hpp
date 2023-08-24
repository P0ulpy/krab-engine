//
// Created by Flo on 26/11/2022.
//

#pragma once

#include "SFML/System/Clock.hpp"
#include "Core.hpp"

namespace KrabEngine
{
    using Timestep = float;

    /**
     * @brief The Time class is a singleton that store the delta time.
     * The delta time is the time between two frames.
     */
    class Time
    {
    public:
        static Time* Get();

    public:
        Time() = default;

        Timestep RestartDeltaTimeClock();
        [[nodiscard]] Timestep GetDeltaTime() const { return s_deltaTime; }
    private:
        sf::Clock m_deltaTimeClock {};
        Timestep s_deltaTime = .0f;
    };
}
