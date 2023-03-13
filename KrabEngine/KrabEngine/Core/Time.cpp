//
// Created by Flo on 15/12/2022.
//

#include "Time.hpp"

namespace KrabEngine
{
    Time* Time::Get()
    {
        static Time instance;
        return &instance;
    }

    Timestep Time::RestartDeltaTimeClock()
    {
        s_deltaTime = (Timestep) m_deltaTimeClock.restart().asMilliseconds();
        return s_deltaTime;
    }
}