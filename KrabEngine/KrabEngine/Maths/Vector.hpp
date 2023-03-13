#pragma once

#include <array>
#include <complex>
#include <iostream>

namespace Maths {
    // @todo maybe inherit Vector and Point from std::array
    // @todo replace for loops with constexpr sequences
    template <typename T, size_t Size>
    struct Vector;

    template <typename T, size_t Size>
    struct Point
    {
        static_assert(Size >= 2);

        template<typename ...VT>
        explicit Point(VT... Vals) {
            Values = {Vals...};
        }

        std::array<T, Size> Values;

        const T& operator[](const int& Index) const
        {
            return Values[Index];
        }

        T& operator[](const int& Index)
        {
            return Values[Index];
        }

        bool operator==(const Point& Other) const
        {
            for (size_t i = 0; i < Size; i++)
            {
                if (Values[i] != Other[i])
                    return false;
            }
            return true;
        }

        bool operator!=(const Point& Other) const
        {
            return !(*this == Other);
        }

        Point operator+(const Vector<T, Size>& Other) const;

        Point operator-(const Vector<T, Size>& Other) const;

        Vector<T, Size> GetVectorTo(const Point& Other) const;

        const T& GetX() const
        {
            return this->Values[0];
        }

        T& GetX()
        {
            return this->Values[0];
        }

        const T& GetY() const
        {
            return this->Values[1];
        }

        T& GetY()
        {
            return this->Values[1];
        }

        template <size_t S = Size, std::enable_if_t<(S >= 3), bool> = false>
        const T& GetZ() const
        {
            return this->Values[2];
        }

        template <size_t S = Size, std::enable_if_t<(S >= 3), bool> = false>
        T& GetZ()
        {
            return this->Values[2];
        }

        template <typename OST, size_t OSSize>
        friend std::ostream& operator<<(std::ostream& os, const Point& p);
    };

    template <typename T, size_t Size>
    std::ostream& operator<<(std::ostream& os, const Point<T, Size>& p)
    {
        for (const auto& V : p.Values)
        {
            os << V << ".";
        }
        return os;
    }

    template <typename T, size_t Size>
    struct Vector
    {
        static_assert(Size >= 2);

        template<typename ...VT>
        explicit Vector(VT... Vals) {
            Values = {Vals...};
        }

        std::array<T, Size> Values;

        const T& operator[](const int& Index) const
        {
            return Values[Index];
        }

        T& operator[](const int& Index)
        {
            return Values[Index];
        }

        bool operator==(const Vector& Other) const
        {
            for (size_t i = 0; i < Size; i++)
            {
                if (Values[i] != Other[i])
                    return false;
            }
            return true;
        }

        bool operator!=(const Vector& Other) const
        {
            return !(*this == Other);
        }

        T GetSquareLength() const
        {
            return this->Scalar(*this);
        }

        T GetLength() const
        {
            return std::sqrt(GetSquareLength());
        }

        Vector GetNormalized() const
        {
            const auto length = GetLength();
            Vector normalized = *this;
            if (length == 0)
                return normalized;
            for (size_t i = 0; i < Size; i++)
            {
                normalized.Values[i] = normalized.Values[i] / length;
            }
            return normalized;
        }

        void Normalize()
        {
            const auto length = GetLength();
            if (length == 0)
                return;
            for (size_t i = 0; i < Size; i++)
            {
                this->Values[i] = this->Values[i] / length;
            }
        }

        Vector operator+(const Vector& Other) const
        {
            Vector newVec;
            for (size_t i = 0; i < Size; i++)
            {
                newVec[i] = Values[i] + Other[i];
            }
            return newVec;
        }

        Vector& operator+=(const Vector& Other)
        {
            for (size_t i = 0; i < Size; i++)
            {
                this->Values[i] += Other[i];
            }
            return *this;
        }

        Vector operator-(const Vector<T, Size>& Other) const
        {
            Vector newVec;
            for (size_t i = 0; i < Size; i++)
            {
                newVec[i] = Values[i] - Other[i];
            }
            return newVec;
        }

        Vector& operator-=(const Vector<T, Size>& Other)
        {
            for (size_t i = 0; i < Size; i++)
            {
                this->Values[i] -= Other[i];
            }
            return *this;
        }

        Vector operator+(const T& Other) const
        {
            Vector newVec;
            for (size_t i = 0; i < Size; i++)
            {
                newVec[i] = Values[i] + Other;
            }
            return newVec;
        }

        Vector& operator+=(const T& Other)
        {
            for (size_t i = 0; i < Size; i++)
            {
                this->Values[i] += Other;
            }
            return *this;
        }

        Vector operator-(const T& Other) const
        {
            Vector newVec;
            for (size_t i = 0; i < Size; i++)
            {
                newVec[i] = Values[i] - Other;
            }
            return newVec;
        }

        Vector& operator-=(const T& Other)
        {
            for (size_t i = 0; i < Size; i++)
            {
                this->Values[i] -= Other;
            }
            return *this;
        }

        Vector operator*(const Vector& Other) const
        {
            Vector newVec;
            for (size_t i = 0; i < Size; i++)
            {
                newVec[i] = Values[i] * Other[i];
            }
            return newVec;
        }

        Vector operator*(const T& Other) const
        {
            Vector newVec;
            for (size_t i = 0; i < Size; i++)
            {
                newVec[i] = Values[i] * Other;
            }
            return newVec;
        }

        Vector operator/(const Vector& Other) const
        {
            return *this * (T(1) / Other);
        }

        Vector operator/(const T& Other) const
        {
            return *this * (T(1) / Other);
        }

        T Scalar(const Vector& Other) const
        {
            T scalar = T(0);
            for (size_t i = 0; i < Size; i++)
            {
                scalar += Values[i] * Other[i];
            }
            return scalar;
        }

        T Scalar(const T& Other) const
        {
            T scalar = T(0);
            for (size_t i = 0; i < Size; i++)
            {
                scalar += Values[i] * Other;
            }
            return scalar;
        }

        template <size_t S = Size, std::enable_if_t<(S == 2), bool> = false>
        T operator^(const Vector& Other) const
        {
            return Values[0] * Other[1] - Values[1] * Other[0];
        }

        template <size_t S = Size, std::enable_if_t<(S == 2), bool> = false>
        Vector operator^(const T& Other) const
        {
            return Vector(
                Other * Values[1],
                -Other * Values[0]
            );
        }

        template <size_t S = Size, std::enable_if_t<(S == 3), bool> = false>
        Vector operator^(const Vector& Other) const
        {
            Vector newVec;
            for (size_t i = 0; i < Size; i++)
            {
                const auto nextIndex = (i + 1) % Size;
                newVec[i] = Values[i] * Other[nextIndex] - Values[nextIndex] * Other[i];
            }
            return newVec;
        }

        static std::array<Vector, Size> GetAxes()
        {
            // should only be called once
            static auto Axes = []()->std::array<Vector, Size>
            {
                std::array<Vector, Size> CreatedAxes;
                for (size_t i = 0; i < Size; i++)
                {
                    static std::array<T, Size> Values;
                    for (size_t v = 0; v < Size; v++)
                    {
                        Values[v] = T(0);
                    }
                    Values[i] = T(1);
                    CreatedAxes[i] = Vector{Values};
                }
                return CreatedAxes;
            }();
            return Axes;
        }

        static Vector GetUnitVectorOnAxis(size_t Axis)
        {
            return GetAxes()[Axis];
        }

        const T& GetX() const
        {
            return this->Values[0];
        }

        T& GetX()
        {
            return this->Values[0];
        }

        const T& GetY() const
        {
            return this->Values[1];
        }

        T& GetY()
        {
            return this->Values[1];
        }

        template <size_t S = Size, std::enable_if_t<(S >= 3), bool> = false>
        const T& GetZ() const
        {
            return this->Values[2];
        }

        template <size_t S = Size, std::enable_if_t<(S >= 3), bool> = false>
        T& GetZ()
        {
            return this->Values[2];
        }

        template <typename OST, size_t OSSize>
        friend std::ostream& operator<<(std::ostream& os, const Vector& v);
    };

    template <typename T, size_t Size>
    std::ostream& operator<<(std::ostream& os, const Vector<T, Size>& v)
    {
        for (const auto& V : v.Values)
        {
            os << V << ".";
        }
        return os;
    }

    template <typename T, size_t Size>
    Point<T, Size> Point<T, Size>::operator+(const Vector<T, Size>& Other) const
    {
        Point newPoint;
        for (size_t i = 0; i < Size; i++)
        {
            newPoint[i] = Values[i] + Other[i];
        }
        return newPoint;
    }

    template <typename T, size_t Size>
    Point<T, Size> Point<T, Size>::operator-(const Vector<T, Size>& Other) const
    {
        Point newPoint;
        for (size_t i = 0; i < Size; i++)
        {
            newPoint[i] = Values[i] - Other[i];
        }
        return newPoint;
    }

    template <typename T, size_t Size>
    Vector<T, Size> Point<T, Size>::GetVectorTo(const Point<T, Size>& Other) const
    {
        Vector<T, Size> newVec;
        for (size_t i = 0; i < Size; i++)
        {
            newVec[i] = Other[i] - Values[i];
        }
        return newVec;
    }

    template <typename T>
    using Point2D = Point<T, 2>;
    template <typename T>
    using Vector2D = Vector<T, 2>;

    template <typename T, size_t Size, bool IsNormalized = true>
    T ProjectOnUnitVector(const Point<T, Size>& Point, const Vector<T, Size>& UnitVector)
    {
        auto NormalizedUnitVector = UnitVector;
        if constexpr (!IsNormalized)
        {
            NormalizedUnitVector = UnitVector.GetNormalized();
        }
        return (Vector<T, Size>{Point.Values}).Scalar(NormalizedUnitVector);
    }

    template <typename T, size_t Size, bool IsNormalized = true>
    Point<T, Size> ProjectOnUnitVectorPoint(const Point<T, Size>& Point, const Vector<T, Size>& UnitVector)
    {
        auto NormalizedUnitVector = UnitVector;
        if constexpr (!IsNormalized)
        {
            NormalizedUnitVector = UnitVector.GetNormalized();
        }
        T Scalar = ProjectOnUnitVector<T, Size, true>(Point, NormalizedUnitVector);
        return NormalizedUnitVector * Scalar;
    }

    template <typename T, size_t Size, bool IsNormalized = true>
    T ProjectOnUnitVector(const Vector<T, Size>& Vec, const Vector<T, Size>& UnitVector)
    {
        auto NormalizedUnitVector = UnitVector;
        if constexpr (!IsNormalized)
        {
            NormalizedUnitVector = UnitVector.GetNormalized();
        }
        return Vec.Scalar(NormalizedUnitVector);
    }

    template <typename T, size_t Size, bool IsNormalized = true>
    Vector<T, Size> ProjectOnUnitVectorVector(const Vector<T, Size>& Vec, const Vector<T, Size>& UnitVector)
    {
        auto NormalizedUnitVector = UnitVector;
        if constexpr (!IsNormalized)
        {
            NormalizedUnitVector = UnitVector.GetNormalized();
        }
        T Scalar = ProjectOnUnitVector<T, Size, true>(Vec, NormalizedUnitVector);
        return NormalizedUnitVector * Scalar;
    }

    template <typename T, size_t Dimensions>
    Maths::Vector<T, Dimensions> GetNormal(Maths::Vector<T, Dimensions> Edge)
    {
        Maths::Vector<T, Dimensions> Normal;
        if constexpr (Dimensions == 2)
        {
            Normal = Edge ^ -1;
        } else {
            static const auto UnitAxisVector = Vector<T, Dimensions>::GetUnitVectorOnAxis(0);
            Normal = Normal ^ UnitAxisVector;
        }
        Normal.Normalize();
        return Normal;
    }
}
