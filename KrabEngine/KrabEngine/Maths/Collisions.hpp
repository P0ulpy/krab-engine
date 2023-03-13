#pragma once

#include <vector>
#include <algorithm>
#include "Vector.hpp"
#include "Shape.hpp"

namespace Maths::Collisions
{
    template <typename GeometricT, size_t Dimensions>
    struct CollisionManifold
    {
        Maths::Vector<GeometricT, Dimensions> Normal;
        GeometricT Depth = GeometricT(0);
        std::vector<Maths::Point<GeometricT, Dimensions>> Contacts;

        bool HasCollided() const
        {
            return !Contacts.empty();
        }
    };

    template <typename GeometricType, size_t Dimensions>
    void GetMinAndMaxProjectedValues(
        const Maths::Vector<GeometricType, Dimensions>& Axis,
        const std::vector<Maths::Point<GeometricType, Dimensions>>& Points,
        GeometricType& Min,
        GeometricType& Max,
        Maths::Point<GeometricType, Dimensions>& PointMin,
        Maths::Point<GeometricType, Dimensions>& PointMax
    )
    {
        Min = Max = Maths::ProjectOnUnitVector(Points[0], Axis);
        PointMin = PointMax = Points[0];
        for (int i = 1; i < Points.size(); i++)
        {
            GeometricType Projection = Maths::ProjectOnUnitVector(Points[i], Axis);
            if (Projection > Max)
            {
                Max = Projection;
                PointMax = Points[i];
                continue;
            }
            if (Projection < Min)
            {
                Min = Projection;
                PointMin = Points[i];
                continue;
            }
        }
    }

    template <typename GeometricType, size_t Dimensions>
    void GetMinProjectedValues(
        const Maths::Vector<GeometricType, Dimensions>& Axis,
        const std::vector<Maths::Point<GeometricType, Dimensions>>& Points,
        GeometricType& Min,
        Maths::Point<GeometricType, Dimensions>& PointMin
    )
    {
        Min = Maths::ProjectOnUnitVector(Points[0], Axis);
        PointMin = Points[0];
        for (int i = 1; i < Points.size(); i++)
        {
            GeometricType Projection = Maths::ProjectOnUnitVector(Points[i], Axis);
            if (Projection < Min)
            {
                Min = Projection;
                PointMin = Points[i];
                continue;
            }
        }
    }

    class SAP {
    public:
        template <typename GeometricType>
        static Maths::Point2D<GeometricType> GetSmallestOnAxis(
            const Maths::Vector2D<GeometricType>& Axis,
            const Maths::Vector2D<GeometricType>& Scale,
            const GeometricType& RotationDegrees,
            const Maths::Point2D<GeometricType>& OnPos,
            const Maths::Rectangle2D<GeometricType>* Shape
        )
        {
            const auto Points = Shape->GetVertices(RotationDegrees, Scale, OnPos);
            GeometricType Min = 0, Max = 0;
            Maths::Point2D<GeometricType> MinPoint, MaxPoint;
            GetMinAndMaxProjectedValues(Axis, Points, Min, Max, MinPoint, MaxPoint);
        }

        template <typename GeometricType, size_t Dimensions>
        static bool LikelyToCollide(
            const Maths::Vector<GeometricType, Dimensions>& Axis,
            const std::vector<Maths::Point<GeometricType, Dimensions>>& PointsA,
            const std::vector<Maths::Point<GeometricType, Dimensions>>& PointsB
        )
        {
            return IntersectOnAxis(
                Axis,
                PointsA,
                PointsB
            );
        }

        // RTTI casting
        template <typename GeometricType, size_t Dimensions>
        static bool LikelyToCollide(
            const Maths::Vector<GeometricType, Dimensions>& Axis,
            const Maths::Vector<GeometricType, Dimensions>& ScaleA,
            const Maths::Vector<GeometricType, Dimensions>& ScaleB,
            const GeometricType& RotationDegreesA,
            const GeometricType& RotationDegreesB,
            const Maths::Point2D<GeometricType>& OnPosA,
            const Maths::Point2D<GeometricType>& OnPosB,
            const Maths::IShape* ShapeA,
            const Maths::IShape* ShapeB
        )
        {
            if (
                ShapeA->getType()->isInstanceOrChildOf(Maths::Rectangle2D<GeometricType>::getClassType())
                && ShapeB->getType()->isInstanceOrChildOf(Maths::Rectangle2D<GeometricType>::getClassType())
            )
            {
                return LikelyToCollide<GeometricType, Dimensions>(
                    Axis,
                    static_cast<const Maths::Rectangle2D<GeometricType>*>(ShapeA)->GetVertices(
                        RotationDegreesA,
                        ScaleA,
                        OnPosA
                    ),
                    static_cast<const Maths::Rectangle2D<GeometricType>*>(ShapeB)->GetVertices(
                        RotationDegreesB,
                        ScaleB,
                        OnPosB
                    )
                );
            }

            throw std::runtime_error("No matching function found.");
        }

        template <typename GeometricType, size_t Dimensions>
        static Maths::Point<GeometricType, Dimensions> GetSmallestOnAxis(
            const Maths::Vector<GeometricType, Dimensions>& Axis,
            const Maths::Vector<GeometricType, Dimensions>& Scale,
            const GeometricType& RotationDegrees,
            const Maths::Point<GeometricType, Dimensions>& OnPos,
            const Maths::IShape* Shape
        )
        {
            if (
                Shape->getType()->isInstanceOrChildOf(Maths::Rectangle2D<GeometricType>::getClassType())
            )
            {
                return GetSmallestOnAxis<GeometricType>(
                    Axis,
                    Scale,
                    RotationDegrees,
                    OnPos,
                    static_cast<const Maths::Rectangle2D<GeometricType>*>(Shape)
                );
            }

            throw std::runtime_error("No matching function found.");
        }
    private:
        template <typename GeometricType, size_t Dimensions>
        static bool IntersectOnAxis(
            const Maths::Vector<GeometricType, Dimensions>& Axis,
            const std::vector<Maths::Point<GeometricType, Dimensions>>& PointsA,
            const std::vector<Maths::Point<GeometricType, Dimensions>>& PointsB
        )
        {
            GeometricType MinA = 0, MaxA = 0, MinB = 0, MaxB = 0;
            Maths::Point<GeometricType, Dimensions> MinPointA, MaxPointA, MinPointB, MaxPointB;
            GetMinAndMaxProjectedValues(Axis, PointsA, MinA, MaxA, MinPointA, MaxPointA);
            GetMinAndMaxProjectedValues(Axis, PointsB, MinB, MaxB, MinPointB, MaxPointB);
            return (MaxA >= MinB) && (MaxB >= MinA);
        }
    };

    class SAT {
    public:
        template <typename GeometricType, size_t Dimensions>
        static CollisionManifold<GeometricType, Dimensions> CollideRectangles(
            const std::vector<Maths::Vector2D<GeometricType>>& Axes,
            const std::vector<Maths::Point2D<GeometricType>>& PointsA,
            const std::vector<Maths::Point2D<GeometricType>>& PointsB
        )
        {
            return CheckCollisions(
                Axes,
                PointsA,
                PointsB
            );
        }

        template <typename GeometricType, size_t Dimensions>
        static CollisionManifold<GeometricType, Dimensions> CollideRectangles(
            const std::vector<Maths::Vector2D<GeometricType>>& NormalsA,
            const std::vector<Maths::Vector2D<GeometricType>>& NormalsB,
            const std::vector<Maths::Point2D<GeometricType>>& PointsA,
            const std::vector<Maths::Point2D<GeometricType>>& PointsB
        )
        {
            std::vector<Maths::Vector2D<GeometricType>> Axes;
            ComputeAxesToCheck(NormalsA, NormalsB, Axes);

            return CollideRectangles<GeometricType, Dimensions>(
                Axes,
                PointsA,
                PointsB
            );
        }

        template <typename GeometricType, size_t Dimensions>
        static CollisionManifold<GeometricType, Dimensions> CollideRectangles(
            const std::vector<Maths::Point2D<GeometricType>>& PointsA,
            const std::vector<Maths::Point2D<GeometricType>>& PointsB,
            const Maths::Rectangle2D<GeometricType>* RectangleA,
            const Maths::Rectangle2D<GeometricType>* RectangleB
        )
        {
            std::vector<Maths::Vector2D<GeometricType>> Axes;
            ComputeAxesToCheck(PointsA, PointsB, RectangleA, RectangleB, Axes);

            return CollideRectangles<GeometricType, Dimensions>(
                Axes,
                PointsA,
                PointsB
            );
        }

        template <typename GeometricType, size_t Dimensions>
        static CollisionManifold<GeometricType, Dimensions> CollideRectangles(
            const Maths::Vector2D<GeometricType>& ScaleA,
            const Maths::Vector2D<GeometricType>& ScaleB,
            const GeometricType& RotationDegreesA,
            const GeometricType& RotationDegreesB,
            const Maths::Point2D<GeometricType>& OnPosA,
            const Maths::Point2D<GeometricType>& OnPosB,
            const Maths::Rectangle2D<GeometricType>* RectangleA,
            const Maths::Rectangle2D<GeometricType>* RectangleB
        )
        {
            std::vector<Maths::Vector2D<GeometricType>> Axes;
            std::vector<Maths::Point2D<GeometricType>> PointsA;
            std::vector<Maths::Point2D<GeometricType>> PointsB;
            ComputeAxesAndPointsToCheck(
                ScaleA,
                ScaleB,
                RotationDegreesA,
                RotationDegreesB,
                OnPosA,
                OnPosB,
                RectangleA,
                RectangleB,
                Axes,
                PointsA,
                PointsB
            );

            return CollideRectangles<GeometricType, Dimensions>(
                Axes,
                PointsA,
                PointsB
            );
        }

        // RTTI casting
        template <typename GeometricType, size_t Dimensions>
        static CollisionManifold<GeometricType, Dimensions> Collide(
            const Maths::Vector<GeometricType, Dimensions>& ScaleA,
            const Maths::Vector<GeometricType, Dimensions>& ScaleB,
            const GeometricType& RotationDegreesA,
            const GeometricType& RotationDegreesB,
            const Maths::Point2D<GeometricType>& OnPosA,
            const Maths::Point2D<GeometricType>& OnPosB,
            const Maths::IShape* ShapeA,
            const Maths::IShape* ShapeB
        )
        {
            if (
                ShapeA->getType()->isInstanceOrChildOf(Maths::Rectangle2D<GeometricType>::getClassType())
                && ShapeB->getType()->isInstanceOrChildOf(Maths::Rectangle2D<GeometricType>::getClassType())
            )
            {
                return CollideRectangles<GeometricType, Dimensions>(
                    ScaleA,
                    ScaleB,
                    RotationDegreesA,
                    RotationDegreesB,
                    OnPosA,
                    OnPosB,
                    static_cast<const Maths::Rectangle2D<GeometricType>*>(ShapeA),
                    static_cast<const Maths::Rectangle2D<GeometricType>*>(ShapeB)
                );
            }

            throw std::runtime_error("No matching function found.");
        }

    private:
        SAT() = default;

        template <typename GeometricType>
        static void ComputeAxesToCheck(
            const std::vector<Maths::Vector2D<GeometricType>>& NormalsA,
            const std::vector<Maths::Vector2D<GeometricType>>& NormalsB,
            std::vector<Maths::Vector2D<GeometricType>>& Axes
        )
        {
            Axes.clear();

            bool Parallel = false;
            for (size_t i = 0; i < NormalsA.size(); i++)
            {
                if (NormalsB[0] == NormalsA[i])
                {
                    Parallel = true;
                    break;
                }
            }

            if (Parallel) {
                const size_t ToReserve = std::floor(NormalsA.size() / 2);
                Axes.reserve(ToReserve);
                Axes.push_back(NormalsA[0]);
                Axes.push_back(NormalsA[1]);
            } else {
                Axes.reserve(NormalsA.size() + NormalsB.size());
                Axes.insert(Axes.begin(), NormalsA.begin(), NormalsA.end());
                Axes.insert(Axes.begin(), NormalsB.begin(), NormalsB.end());
            }
        }

        template <typename GeometricType>
        static void ComputeAxesToCheck(
            const std::vector<Maths::Point2D<GeometricType>>& PointsA,
            const std::vector<Maths::Point2D<GeometricType>>& PointsB,
            const Maths::Rectangle2D<GeometricType>* RectangleA,
            const Maths::Rectangle2D<GeometricType>* RectangleB,
            std::vector<Maths::Vector2D<GeometricType>>& Axes
        )
        {
            Axes.clear();
            auto EdgesA = RectangleA->GetEdges(PointsA);
            auto EdgesB = RectangleB->GetEdges(PointsB);
            auto NormalsA = RectangleA->GetNormals(EdgesA);
            auto NormalsB = RectangleB->GetNormals(EdgesB);

            ComputeAxesToCheck(NormalsA, NormalsB);
        }

        template <typename GeometricType>
        static void ComputeAxesAndPointsToCheck(
            const Maths::Vector2D<GeometricType>& ScaleA,
            const Maths::Vector2D<GeometricType>& ScaleB,
            const GeometricType& RotationDegreesA,
            const GeometricType& RotationDegreesB,
            const Maths::Point2D<GeometricType>& OnPosA,
            const Maths::Point2D<GeometricType>& OnPosB,
            const Maths::Rectangle2D<GeometricType>* RectangleA,
            const Maths::Rectangle2D<GeometricType>* RectangleB,
            std::vector<Maths::Vector2D<GeometricType>>& Axes,
            std::vector<Maths::Point2D<GeometricType>>& PointsA,
            std::vector<Maths::Point2D<GeometricType>>& PointsB
        )
        {
            Axes.clear();
            PointsA.clear();
            PointsB.clear();
            auto VerticesA = RectangleA->GetVertices(RotationDegreesA, ScaleA, OnPosA);
            auto VerticesB = RectangleB->GetVertices(RotationDegreesB, ScaleB, OnPosB);
            auto EdgesA = RectangleA->GetEdges(VerticesA);
            auto EdgesB = RectangleB->GetEdges(VerticesB);
            auto NormalsA = RectangleA->GetNormals(EdgesA);
            auto NormalsB = RectangleB->GetNormals(EdgesB);

            bool Parallel = false;
            for (size_t i = 0; i < NormalsA.size(); i++)
            {
                if (NormalsB[0] == NormalsA[i])
                {
                    Parallel = true;
                    break;
                }
            }

            PointsA.reserve(VerticesA.size());
            PointsB.reserve(VerticesB.size());
            if (Parallel) {
                const size_t ToReserve = std::floor(NormalsA.size() / 2);
                Axes.reserve(ToReserve);
                Axes.push_back(NormalsA[0]);
                Axes.push_back(NormalsA[1]);
            } else {
                Axes.reserve(NormalsA.size() + NormalsB.size());
                Axes.insert(Axes.begin(), NormalsA.begin(), NormalsA.end());
                Axes.insert(Axes.begin(), NormalsB.begin(), NormalsB.end());
            }
            PointsA.insert(PointsA.begin(), VerticesA.begin(), VerticesA.end());
            PointsB.insert(PointsB.begin(), VerticesB.begin(), VerticesB.end());
        }

        // Edges point to MaxPoint
        // Only convex polygons
        template <typename GeometricType, size_t Dimensions>
        static Maths::Vector<GeometricType, Dimensions> GetBestEdgeForCollisionPoint(
            const Maths::Point<GeometricType, Dimensions>& MaxPoint,
            const std::vector<Maths::Point<GeometricType, Dimensions>>& Points,
            const Maths::Vector<GeometricType, Dimensions>& CollisionNormal
        )
        {
            const size_t PointAIndex = std::find(Points.begin(), Points.end(), MaxPoint) - Points.begin();
            const auto Edge1 = Points[PointAIndex == 0 ? Points.size() - 1 : PointAIndex - 1].GetVectorTo(MaxPoint);
            const auto Edge2 = Points[(PointAIndex + 1) % Points.size()].GetVectorTo(MaxPoint);
            if (std::abs(Edge1.GetNormalized().Scalar(CollisionNormal)) <= std::abs(Edge2.GetNormalized().Scalar(CollisionNormal))) {
                return Edge1;
            } else {
                return Edge2;
            }
        }

        // CollisionManifold normal must always point from A to B
        // @todo this only works with 2D ?
        template <typename GeometricType, size_t Dimensions>
        static std::vector<Maths::Point<GeometricType, Dimensions>> GetContactManifold(
            const std::vector<Maths::Point<GeometricType, Dimensions>>& PointsA,
            const std::vector<Maths::Point<GeometricType, Dimensions>>& PointsB,
            const Maths::Vector<GeometricType, Dimensions>& CollisionNormal
        )
        {
            GeometricType CollisionNormalMinA;
            GeometricType CollisionNormalMaxA;
            Maths::Point<GeometricType, Dimensions> MinAPointOnCollisionNormal;
            Maths::Point<GeometricType, Dimensions> MaxAPointOnCollisionNormal;
            GetMinAndMaxProjectedValues(
                CollisionNormal,
                PointsA,
                CollisionNormalMinA,
                CollisionNormalMaxA,
                MinAPointOnCollisionNormal,
                MaxAPointOnCollisionNormal
            );
            GeometricType CollisionNormalMinB;
            GeometricType CollisionNormalMaxB;
            Maths::Point<GeometricType, Dimensions> MinBPointOnCollisionNormal;

            auto FlippedCollisionNormal = CollisionNormal * -1;
            Maths::Point<GeometricType, Dimensions> MaxBPointOnCollisionNormal;
            GetMinAndMaxProjectedValues(
                FlippedCollisionNormal,
                PointsB,
                CollisionNormalMinB,
                CollisionNormalMaxB,
                MinBPointOnCollisionNormal,
                MaxBPointOnCollisionNormal
            );

            auto BestA = GetBestEdgeForCollisionPoint(MaxAPointOnCollisionNormal, PointsA, CollisionNormal);
            auto BestB = GetBestEdgeForCollisionPoint(MaxBPointOnCollisionNormal, PointsB, FlippedCollisionNormal);
            bool Flipped = false;
            auto ReferenceEdge = BestA;
            auto ReferenceMaxPoint = MaxAPointOnCollisionNormal;
            auto IncidentEdge = BestB;
            auto IncidenceMaxPoint = MaxBPointOnCollisionNormal;
            if (std::abs(BestB.Scalar(CollisionNormal)) < std::abs(BestA.Scalar(CollisionNormal)))
            {
                ReferenceEdge = BestB;
                IncidentEdge = BestA;
                Flipped = true;
                ReferenceMaxPoint = MaxBPointOnCollisionNormal;
                IncidenceMaxPoint = MaxAPointOnCollisionNormal;
            }

            // clip first part of the points, moving point along axis to keep it
            auto ReferenceFurtherVertice = ReferenceMaxPoint - ReferenceEdge;
            auto NormalizedRef = ReferenceEdge.GetNormalized();
            GeometricType Max1 = NormalizedRef.Scalar(Maths::Vector<GeometricType, Dimensions>{(ReferenceFurtherVertice).Values});
            auto ClippedPoints = ClipPoints<GeometricType, Dimensions, false>(
                {IncidenceMaxPoint, IncidenceMaxPoint - IncidentEdge},
                NormalizedRef,
                Max1
            );

            // clip second part of the points, moving point along axis to keep it
            GeometricType Max2 = NormalizedRef.Scalar(Maths::Vector<GeometricType, Dimensions>{ReferenceMaxPoint.Values});
            ClippedPoints = ClipPoints<GeometricType, Dimensions, false>(
                {ClippedPoints[0], ClippedPoints[1]},
                NormalizedRef * -1,
                -Max2
            );

//            auto RefNormal = GetNormal(ReferenceEdge);
            auto RefNormal = FlippedCollisionNormal;
            if (Flipped)
                RefNormal = CollisionNormal;

            // clip third part of the points but this time deleting them if they are in a discarded part
            GeometricType Max3 = RefNormal.Scalar(Maths::Vector<GeometricType, Dimensions>{ReferenceMaxPoint.Values});
            ClippedPoints = ClipPoints<GeometricType, Dimensions, true>(
                {ClippedPoints[0], ClippedPoints[1]},
                RefNormal,
                Max3
            );
            return ClippedPoints;
        }

        // OverlapDepth must be initialized with < 0 to correctly compute
        template <typename GeometricType, size_t Dimensions>
        static bool IntersectOnAxis(
            const Maths::Vector<GeometricType, Dimensions>& Axis,
            const std::vector<Maths::Point<GeometricType, Dimensions>>& PointsA,
            const std::vector<Maths::Point<GeometricType, Dimensions>>& PointsB,
            Maths::Vector<GeometricType, Dimensions>& CollisionNormal,
            GeometricType& OverlapDepth
        )
        {
            GeometricType MinA = 0, MaxA = 0, MinB = 0, MaxB = 0;
            Maths::Point<GeometricType, Dimensions> MinPointA, MaxPointA, MinPointB, MaxPointB;
            GetMinAndMaxProjectedValues(Axis, PointsA, MinA, MaxA, MinPointA, MaxPointA);
            GetMinAndMaxProjectedValues(Axis, PointsB, MinB, MaxB, MinPointB, MaxPointB);
            if ((MaxA >= MinB) && (MaxB >= MinA))
            {
                auto Max = std::min(MaxA, MaxB);
                auto Min = std::max(MinA, MinB);
                GeometricType AbsOverlapSize = std::abs(Max - Min);
                if (OverlapDepth < 0 || AbsOverlapSize < OverlapDepth)
                {
                    OverlapDepth = AbsOverlapSize;
                    // CollisionManifold normal must always point from B to A
                    auto Modifier = GeometricType(1.f);
                    if (MaxA < MaxB && MinA < MinB)
                        Modifier *= -1;
                    CollisionNormal = Axis * Modifier;
                }
                return true;
            }

            return false;
        }

        template <typename GeometricType, size_t Dimensions, bool RemovePoints = false>
        static std::vector<Maths::Point<GeometricType, Dimensions>> ClipPoints(
            const std::array<Maths::Point<GeometricType, Dimensions>, 2>& Points,
            const Maths::Vector<GeometricType, Dimensions>& Axis,
            const GeometricType& Max
        )
        {
            std::vector<Maths::Point<GeometricType, Dimensions>> FinalPoints;
            FinalPoints.reserve(2);

            GeometricType D1 = Axis.Scalar(Maths::Vector<GeometricType, Dimensions>{Points[0].Values}) - Max;
            GeometricType D2 = Axis.Scalar(Maths::Vector<GeometricType, Dimensions>{Points[1].Values}) - Max;

            if (D1 >= GeometricType(0)) FinalPoints.push_back(Points[0]);
            if (D2 >= GeometricType(0)) FinalPoints.push_back(Points[1]);

            if constexpr (!RemovePoints) {
                if (D1 * D2 < GeometricType(0))
                {
                    Maths::Vector<GeometricType, Dimensions> Edge = Points[0].GetVectorTo(Points[1]);
                    GeometricType Location = D1 / (D1 - D2);
                    FinalPoints.push_back(
                        Maths::Point<GeometricType, Dimensions>{(Edge * Location + Maths::Vector<GeometricType, Dimensions>{Points[0].Values}).Values}
                    );
                }
            }

            return FinalPoints;
        }

        template <typename GeometricType, size_t Dimensions>
        static CollisionManifold<GeometricType, Dimensions> CheckCollisions(
            const std::vector<Maths::Vector<GeometricType, Dimensions>>& Axes,
            const std::vector<Maths::Point<GeometricType, Dimensions>>& PointsA,
            const std::vector<Maths::Point<GeometricType, Dimensions>>& PointsB
        )
        {
            CollisionManifold<GeometricType, Dimensions> Manifold;
            Maths::Vector<GeometricType, Dimensions> CollisionNormal;
            GeometricType OverlapDepth = -1;
            for (const auto& Axis : Axes)
            {
                if (!IntersectOnAxis(
                    Axis,
                    PointsA,
                    PointsB,
                    CollisionNormal,
                    OverlapDepth
                ))
                {
                    return Manifold;
                }
            }

            Manifold.Depth = OverlapDepth;
            Manifold.Normal = CollisionNormal;
            Manifold.Contacts = GetContactManifold(PointsA, PointsB, CollisionNormal * -1);

            return Manifold;
        }
    };
} // Collisions // Maths