#pragma once

#ifdef WIN32
    #include "corecrt_math_defines.h"
#endif
#include <cmath>

#include "Vector.hpp"

namespace Maths::Angles
{
    template <typename T>
    T DegToRad(T Degrees)
    {
        return Degrees * (M_PI / 180);
    }

    template <typename T>
    T RadToDeg(T Radians)
    {
        return Radians * 180 / M_PI;
    }

    template <typename T>
    Maths::Vector2D<T> RotateVector(const Maths::Vector2D<T>& Vector, T Radians)
    {
        const T cos = std::cos(Radians);
        const T sin = std::sin(Radians);
        return Maths::Vector2D<T>(
            Vector.GetX() * cos + Vector.GetY() * -sin,
            Vector.GetX() * sin + Vector.GetY() * cos
        );
    }
} // Angles // Maths