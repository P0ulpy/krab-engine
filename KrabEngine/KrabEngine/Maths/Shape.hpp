#pragma once

#include <vector>
#include "Vector.hpp"
#include "../RTTI/ClassType.hpp"
#include "Angles.hpp"

namespace Maths {
    struct IShape : public RTTI::IClassType
    {
        DECLARE_CLASS_TYPE(IShape, RTTI::NoClassTypeAncestor);
    };

    template <typename T, size_t Dimensions>
    struct NSphere : public IShape
    {
        DECLARE_CLASS_TYPE(NSphere, IShape);
    };

    template <typename T, size_t Dimensions>
    struct Polygon : public IShape
    {
        DECLARE_CLASS_TYPE(Polygon, IShape);

        virtual std::vector<Point<T, Dimensions>> GetVertices(
            const T& RotationDegrees,
            const Vector<T, Dimensions>& Scale,
            const Point<T, Dimensions>& OnPos
        ) const = 0;

        std::vector<Vector<T, Dimensions>> GetEdges(
            const T& RotationDegrees,
            const Vector<T, Dimensions>& Scale,
            const Point<T, Dimensions>& OnPos
        ) const
        {
            return GetEdges(GetVertices(RotationDegrees, Scale, OnPos));
        }

        std::vector<Vector<T, Dimensions>> GetEdges(
            const std::vector<Point<T, Dimensions>>& Vertices
        ) const
        {
            std::vector<Vector<T, Dimensions>> Edges = {};
            Edges.resize(Vertices.size());
            for (size_t i = 1; i <= Vertices.size(); i++)
            {
                const auto CurrentIdx = i % (Vertices.size());
                Edges[CurrentIdx] = Vertices[i - 1].GetVectorTo(Vertices[CurrentIdx]);
            }
            return Edges;
        }

        std::vector<Vector<T, Dimensions>> GetNormals(
            const T& RotationDegrees,
            const Vector<T, Dimensions>& Scale,
            const Point<T, Dimensions>& OnPos
        ) const
        {
            return GetNormals(GetEdges(RotationDegrees, Scale, OnPos));
        }

        std::vector<Vector<T, Dimensions>> GetNormals(
            const std::vector<Vector<T, Dimensions>>& Edges
        ) const
        {
            auto Normals = Edges;
            for (size_t i = 0; i < Normals.size(); i++)
            {
                Normals[i] = GetNormal(Edges[i]);
            }

            return Normals;
        }
    };

    template <typename T>
    struct Rectangle2D : public Polygon<T, 2>
    {
        using PolygonType = Polygon<T, 2>;
        DECLARE_CLASS_TYPE(Rectangle2D, PolygonType);

        T Width;
        T Height;

        T ComputeMaxDistanceFromPosition()
        {
            T HalfHeight = Height / 2;
            T HalfWidth = Width / 2;
            // half diagonal
            return std::sqrt(HalfWidth * HalfWidth + HalfHeight * HalfHeight);
        }

        std::vector<Point2D<T>> GetRotatedVertices(
            const T& RotationDegrees,
            const Vector2D<T>& Scale
        ) const
        {
            const auto RotationRadians = Maths::Angles::DegToRad(RotationDegrees);
            const auto WidthWithScale = Width * Scale.GetX();
            const auto HeightWithScale = Height * Scale.GetY();
            const auto VecForCenter = Vector2D<T>(
                WidthWithScale / 2,
                HeightWithScale / 2
            );
            return {
                Point2D<T>{
                    (Maths::Angles::RotateVector(
                        Vector2D<T>(T(0), T(0)) - VecForCenter,
                        RotationRadians
                    )).Values
                },
                Point2D<T>{
                    (Maths::Angles::RotateVector(
                        Vector2D<T>(WidthWithScale, T(0)) - VecForCenter,
                        RotationRadians
                    )).Values
                },
                Point2D<T>{
                    (Maths::Angles::RotateVector(
                        Vector2D<T>(WidthWithScale, HeightWithScale) - VecForCenter,
                        RotationRadians
                    )).Values
                },
                Point2D<T>{
                    (Maths::Angles::RotateVector(
                        Vector2D<T>(T(0), HeightWithScale) - VecForCenter,
                        RotationRadians
                    )).Values
                },
            };
        }

        std::vector<Point2D<T>> GetVertices(
            const T& RotationDegrees,
            const Vector2D<T>& Scale,
            const Point2D<T>& OnPos
        ) const override
        {
            const auto VecPos = Vector2D<T>{OnPos.Values};
            auto RotatedVertices = GetRotatedVertices(RotationDegrees , Scale);
            for (size_t i = 0; i < RotatedVertices.size(); i++)
            {
                RotatedVertices[i] = RotatedVertices[i] + VecPos;
            }
            return RotatedVertices;
        }

        virtual bool IsPointInside(const Point2D<T>& Point) const
        {
            return Point.GetX() >= 0
                   && Point.GetX() <= Width
                   && Point.GetY() >= 0
                   && Point.GetY() <= Height
                ;
        }
    };

    template <typename T>
    struct Circle2D : public NSphere<T, 2>
    {
        using NSphereType = NSphere<T, 2>;
        DECLARE_CLASS_TYPE(Rectangle2D, NSphereType);

        T Radius;

        T ComputeMaxDistanceFromPosition()
        {
            return Radius;
        }
    };

    template <typename T>
    struct IBoundingBox2D
    {
        Point2D<T> Position = Point2D<T>(T(0), T(0));
        T Angle = 0;

        virtual T ComputeMaxDistanceFromPosition() = 0;
        virtual ~IBoundingBox2D() = default;
    };

    template <typename T, template <typename ShapeT> typename Shape>
    struct BoundingBox2D : public Shape<T>, public IBoundingBox2D<T>
    {
        using ShapeT = Shape<T>;

        DECLARE_CLASS_TYPE(BoundingBox2D, Shape<T>);

        T ComputeMaxDistanceFromPosition() override
        {
            return Shape<T>::ComputeMaxDistanceFromPosition();
        }
    };

    template <typename T>
    struct RectangleBoundingBox2D : public BoundingBox2D<T, Rectangle2D>
    {
        std::vector<Point2D<T>> GetVertices(
            const T& RotationDegrees,
            const Vector2D<T>& Scale,
            const Point2D<T>& OnPos
        ) const
        {
            const auto VecFromPos = Vector2D<T>{this->Position.Values};
            return Rectangle2D<T>::GetVertices(
                this->Angle + RotationDegrees,
                Scale,
                OnPos + VecFromPos
            );
        }
    };

    template <typename T>
    using CircleBoundingBox2D = BoundingBox2D<T, Circle2D>;

    template <typename T, size_t Dimensions>
    Maths::Point<T, Dimensions> GetCentroid(std::vector<Maths::Point<T, Dimensions>> OfPoints) {
        Maths::Point<T, Dimensions> Centroid;

        for (const auto& Point : OfPoints)
        {
            for (size_t i = 0; i < Dimensions; i++)
            {
                Centroid[i] += Point[i];
            }
        }

        int PointsCount = OfPoints.size();
        for (size_t i = 0; i < Dimensions; i++)
        {
            Centroid[i] /= PointsCount;
        }

        return Centroid;
    }
};
