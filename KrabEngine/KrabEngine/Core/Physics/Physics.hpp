#pragma once

#include <list>

#include "../Components/Physics.hpp"

namespace KrabEngine::Physics {
    using PhysicsPropertiesCombinationType = Engine::Components::Physics::PhysicsPropertiesCombinationType;

//    void CreateCollisionsKdTree()
//    {
//        std::vector<EntityHandle> ToBeOrdered = {};
//        for (auto& [handle, component] : GetSystem<Engine::Components::Physics::RigidBody2DdComponent>()->components) {
//            if (!component.HasCollisions)
//            {
//                continue;
//            }
//            ToBeOrdered.push_back(handle);
//        }
//        PhysicsEntityKdTree.CreateKdTree<2, Maths::Point2D<double>, true>(
//            ToBeOrdered,
//            [this](EntityHandle handle)->Maths::Point2D<double> { return this->GetSystem<Engine::Components::Transform>()->GetOf(handle)->Pos; }
//        );
//    }

    class IPhysicsSimulator
    {
    public:
        virtual void Simulate(
            const float& DeltaTime,
            Engine::EntitiesRegistry* EntitiesRegistry
        ) = 0;
        virtual RTTI::ClassType* GetBodyType() = 0;
        virtual ~IPhysicsSimulator() = default;
    };

    template <typename RigidBodyComponentT>
    class Physics2DSimulator : public IPhysicsSimulator {
    private:
        // 1 mm
        static constexpr RigidBodyComponentT::GeometricT DistanceStep = typename RigidBodyComponentT::GeometricT(0.1f);

        using CollisionManifoldT = Maths::Collisions::CollisionManifold<
            typename RigidBodyComponentT::GeometricT,
            RigidBodyComponentT::Dimensions
        >;

        struct CollisionManifoldWithBodies : public CollisionManifoldT
        {
            RigidBodyComponentT* FirstRigidBody = nullptr;
            RigidBodyComponentT* SecondRigidBody = nullptr;
            Engine::Components::Transform* FirstTransform = nullptr;
            Engine::Components::Transform* SecondTransform = nullptr;
            RigidBodyComponentT::GeometricPointT FirstCenterOfMassAtCollision;
            RigidBodyComponentT::GeometricPointT SecondCenterOfMassAtCollision;
        };

        struct CachedEntity {
            RigidBodyComponentT* Body = nullptr;
            Engine::Components::Transform* Transform = nullptr;
        };

    public:
        void Simulate(
            const float& DeltaTime,
            Engine::EntitiesRegistry* EntitiesRegistry
        ) override
        {
            auto& Components = EntitiesRegistry->GetSystem<RigidBodyComponentT>()->components;
            static std::vector<CachedEntity> Bodies(Components.size());
            Bodies.clear();
            for (auto& [Handle, Component] : Components)
            {
                auto Transform = Component.GetEntityTransform();

                Bodies.push_back({
                    &Component,
                    Transform
                });
            }

            // choose axis Y because with gravity it should be the most reliable for performance
            const auto Axis = RigidBodyComponentT::GeometricVectorT::GetUnitVectorOnAxis(0);

            const auto StepMs = 2.f;
            Timestep Elapsed = 0;
            while (Elapsed < DeltaTime) {
                SimulateRigidBodies(StepMs, Bodies);
                auto SortComp = [&Axis](const CachedEntity& A, const CachedEntity& B)->bool {
                    auto MinA = A.Body->GetRigidBody()->GetMinProjectedOnAxis(Axis);
                    auto MinB = B.Body->GetRigidBody()->GetMinProjectedOnAxis(Axis);
                    return MinA < MinB;
                };
                if (!std::is_sorted(Bodies.begin(), Bodies.end(), SortComp)) {
                    // replace with custom bubble sort
                    std::sort(Bodies.begin(), Bodies.end(), SortComp);
                }

                HandleCollisionsOnBodies(Axis, Bodies);
                Elapsed += StepMs;
            }
            // handle left time
            const auto Left = DeltaTime - Elapsed;
            if (Left > 0) {
                SimulateRigidBodies(Left, Bodies);

                HandleCollisionsOnBodies(Axis, Bodies);
            }
        }

        RTTI::ClassType* GetBodyType() override
        {
            return RigidBodyComponentT::getClassType();
        }
    private:
        void SimulateRigidBodies(
            const float& DeltaTime,
            std::vector<CachedEntity>& SortedBodiesOnAxis
        ) const
        {
            for (auto& CachedEntity : SortedBodiesOnAxis) {
                CachedEntity.Body->SimulateRigidBody(DeltaTime, CachedEntity.Transform);
                CachedEntity.Body->HandleRecomputeCachedProperties();
            }
        }

        void HandleCollisionsOnBodies(
            const typename RigidBodyComponentT::GeometricVectorT& Axis,
            std::vector<CachedEntity>& SortedBodiesOnAxis
        ) const
        {
            static std::vector<std::array<CachedEntity, 2>> LikelyToCollide(SortedBodiesOnAxis.size() / 2);
            LikelyToCollide.clear();
            // list may be good if many operations on list but too much cache miss
            static std::vector<CachedEntity> ActiveIntervals(SortedBodiesOnAxis.size() / 5);
            ActiveIntervals.clear();

            for (auto& CachedEntity : SortedBodiesOnAxis)
            {
                auto Body = CachedEntity.Body;
                auto BodyTransform = CachedEntity.Transform;

                if (!ActiveIntervals.empty()) {
                    for (int i = ActiveIntervals.size() - 1; i >= 0; i--) {
                        auto WithCachedEntity = ActiveIntervals[i];
                        auto WithCachedEntityBody = WithCachedEntity.Body;
                        if (Body->IsStatic && WithCachedEntityBody->IsStatic)
                        {
                            continue;
                        }

                        if (AreCachedEntitiesLikelyToCollide<typename RigidBodyComponentT::GeometricT, RigidBodyComponentT::Dimensions>(
                            Axis,
                            CachedEntity,
                            WithCachedEntity
                        )) {
                            LikelyToCollide.push_back({CachedEntity, WithCachedEntity});
                        } else {
                            // we do not need to keep it as we are only going further
                            ActiveIntervals.erase(ActiveIntervals.begin() + i);
                        }
                    }
                }
                ActiveIntervals.push_back(CachedEntity);
            }

            static std::vector<CollisionManifoldWithBodies> ContactManifolds(LikelyToCollide.size() / 2);
            ContactManifolds.clear();
            for (auto& [CollidingBodyA, CollidingBodyB] : LikelyToCollide)
            {
                CheckAndHandleCollision(
                    CollidingBodyA,
                    CollidingBodyB,
                    ContactManifolds
                );
            }

            for (const auto& Manifold : ContactManifolds)
            {
                // Call OnCollide
                // @todo maybe replace this with bitmasks?
                bool BlockCollideA = Manifold.FirstRigidBody->OnCollide(Manifold.SecondRigidBody, Manifold.Normal);
                bool BlockCollideB = Manifold.SecondRigidBody->OnCollide(Manifold.FirstRigidBody, Manifold.Normal * -1);
                if (!BlockCollideA && !BlockCollideB) {
                    ApplyForceFromCollision(Manifold);
                }
            }
        }

        void CheckAndHandleCollision(
            CachedEntity& CachedEntityA,
            CachedEntity& CachedEntityB,
            std::vector<CollisionManifoldWithBodies>& CollisionManifolds
        ) const
        {
            auto BodyAComponent = CachedEntityA.Body;
            auto BodyBComponent = CachedEntityB.Body;
            BodyAComponent->HandleRecomputeCachedProperties();
            BodyBComponent->HandleRecomputeCachedProperties();
            auto BodyA = BodyAComponent->GetRigidBody();
            auto BodyB = BodyBComponent->GetRigidBody();
            auto BodyATransform = CachedEntityA.Transform;
            auto BodyBTransform = CachedEntityB.Transform;
            if (BodyAComponent->IsStatic && BodyBComponent->IsStatic)
            {
                return;
            }

            auto CollisionManifold = CollideCachedEntities<typename RigidBodyComponentT::GeometricT, RigidBodyComponentT::Dimensions>(
                CachedEntityA,
                CachedEntityB
            );
            if (CollisionManifold.HasCollided()) {
                CollisionManifoldWithBodies Manifold;
                Manifold.Normal = CollisionManifold.Normal;
                Manifold.Contacts = CollisionManifold.Contacts;
                Manifold.Depth = CollisionManifold.Depth;
                Manifold.FirstRigidBody = BodyAComponent;
                Manifold.SecondRigidBody = BodyBComponent;
                Manifold.FirstTransform = BodyATransform;
                Manifold.SecondTransform = BodyBTransform;
                Manifold.FirstCenterOfMassAtCollision = BodyA->GetCenterOfMass(
                    BodyATransform->Scale,
                    BodyATransform->Angle,
                    BodyATransform->Pos
                );
                Manifold.SecondCenterOfMassAtCollision = BodyB->GetCenterOfMass(
                    BodyBTransform->Scale,
                    BodyBTransform->Angle,
                    BodyBTransform->Pos
                );
                CollisionManifolds.push_back(Manifold);
            }
        }

        [[nodiscard]] PhysicsPropertiesCombinationType GetCombinationType(
            const PhysicsPropertiesCombinationType& CombinationTypeA,
            const PhysicsPropertiesCombinationType& CombinationTypeB
        ) const
        {
            return CombinationTypeA <= CombinationTypeB ? CombinationTypeA : CombinationTypeB;
        }

        template <bool Clamp = false>
        RigidBodyComponentT::PhysicsT CombinePhysicsProperties(
            const typename RigidBodyComponentT::PhysicsT& A,
            const typename RigidBodyComponentT::PhysicsT& B,
            const PhysicsPropertiesCombinationType& CombinationTypeA,
            const PhysicsPropertiesCombinationType& CombinationTypeB
        ) const
        {
            const auto CombinationType = GetCombinationType(CombinationTypeA, CombinationTypeB);
            auto Value = A;
            switch (CombinationType) {
                case PhysicsPropertiesCombinationType::MINIMUM:
                    Value = std::min(A, B);
                case PhysicsPropertiesCombinationType::MAXIMUM:
                    Value = std::max(A, B);
                case PhysicsPropertiesCombinationType::MULTIPLY:
                    Value = A * B;
                case PhysicsPropertiesCombinationType::SQUARED:
                    Value = std::sqrt(A * A + B * B);
                default:
                case PhysicsPropertiesCombinationType::AVERAGE:
                    Value = (A + B) / 2;
            }
            if constexpr (!Clamp) return Value;

            return std::clamp(Value, typename RigidBodyComponentT::PhysicsT(0), typename RigidBodyComponentT::PhysicsT(1));
        }

        void ApplyForceFromCollision(
            const CollisionManifoldWithBodies& CollisionManifold
        ) const
        {
            auto BodyAComponent = CollisionManifold.FirstRigidBody;
            auto BodyBComponent = CollisionManifold.SecondRigidBody;
            auto BodyATransform = CollisionManifold.FirstTransform;
            auto BodyBTransform = CollisionManifold.SecondTransform;
            auto BodyA = BodyAComponent->GetRigidBody();
            auto BodyB = BodyBComponent->GetRigidBody();

            if (BodyAComponent->IsStatic && BodyBComponent->IsStatic)
            {
                return;
            }

            const auto OverlapResolvingVector = CollisionManifold.Normal * std::max(CollisionManifold.Depth, DistanceStep);
            if (BodyAComponent->IsStatic) {
                BodyBTransform->Pos = BodyBTransform->Pos + (OverlapResolvingVector * -1);
                BodyBComponent->GetRigidBody()->IsCacheComputed = false;
            } else if (BodyBComponent->IsStatic) {
                BodyATransform->Pos = BodyATransform->Pos + OverlapResolvingVector;
                BodyAComponent->GetRigidBody()->IsCacheComputed = false;
            } else {
                const auto HalfResolve = OverlapResolvingVector * (1 / 2);
                BodyATransform->Pos = BodyATransform->Pos + HalfResolve;
                BodyBTransform->Pos = BodyBTransform->Pos + (HalfResolve * -1);
                BodyAComponent->GetRigidBody()->IsCacheComputed = false;
                BodyBComponent->GetRigidBody()->IsCacheComputed = false;
            }

            // direction from A to B
            const auto Normal = CollisionManifold.Normal * -1;

            const auto ContactPointCentroid = Maths::GetCentroid(CollisionManifold.Contacts);

            // meters
            const auto VecFromCenterOfMassA = (CollisionManifold.FirstCenterOfMassAtCollision.GetVectorTo(ContactPointCentroid) / 100) * (BodyAComponent->CanRotate ? 1.f : 0.f);
            const auto VecFromCenterOfMassB = (CollisionManifold.SecondCenterOfMassAtCollision.GetVectorTo(ContactPointCentroid) / 100) * (BodyBComponent->CanRotate ? 1.f : 0.f);

            auto RelativeVelocity = (
                (BodyB->LinearVelocity + VecFromCenterOfMassB * BodyB->AngularVelocity)
                - (BodyA->LinearVelocity + VecFromCenterOfMassA * BodyA->AngularVelocity)
            );
            auto RelativeVelocityOnNormal = RelativeVelocity.Scalar(Normal);

            if (RelativeVelocityOnNormal > typename RigidBodyComponentT::PhysicsT(0))
            {
                RelativeVelocityOnNormal = 0;
            }

            const auto Restitution = CombinePhysicsProperties(
                BodyAComponent->Material.GetRestitution(),
                BodyBComponent->Material.GetRestitution(),
                BodyAComponent->Material.RestitutionCombinationType,
                BodyBComponent->Material.RestitutionCombinationType
            );

            const auto DistanceFromCenterOfMassAOnNormal = VecFromCenterOfMassA.Scalar(Normal);
            const auto DistanceFromCenterOfMassBOnNormal = VecFromCenterOfMassB.Scalar(Normal);
            const auto AngularAImpulseDenom = DistanceFromCenterOfMassAOnNormal * DistanceFromCenterOfMassAOnNormal * BodyA->InvMomentOfInertiaForZ;
            const auto AngularBImpulseDenom = DistanceFromCenterOfMassBOnNormal * DistanceFromCenterOfMassBOnNormal * BodyB->InvMomentOfInertiaForZ;
            // (-(1 + e)((Va - Vb) ⋅ n)) / ((1 / ma) + (1 / mb) + ((ra ⋅ n)² / Ia) + ((rb ⋅ n)² / Ib))
            auto jN = (-(typename RigidBodyComponentT::PhysicsT(1) + Restitution) * RelativeVelocityOnNormal)
                            / (
                                BodyA->InvMass + BodyB->InvMass
                                + (AngularAImpulseDenom)
                                + (AngularBImpulseDenom)
                            );

            if (jN < 0.000001f)
                jN = 0;

            const auto NormalImpulse = Normal * jN;

            // apply on both bodies
            // (Va2 = Va1 + j / ma ⋅ n)
            BodyA->LinearVelocity = BodyA->LinearVelocity - NormalImpulse * BodyA->InvMass;
            BodyB->LinearVelocity = BodyB->LinearVelocity + NormalImpulse * BodyB->InvMass;

            // (Wa2 = Wa1 + ra ⋅ jn / Ia)
            BodyA->AngularVelocity -= (VecFromCenterOfMassA ^ NormalImpulse) * BodyA->InvMomentOfInertiaForZ;
            BodyB->AngularVelocity += (VecFromCenterOfMassB ^ NormalImpulse) * BodyB->InvMomentOfInertiaForZ;

            const auto StaticFrictionCoef = CombinePhysicsProperties(
                BodyAComponent->Material.GetStaticFriction(),
                BodyBComponent->Material.GetStaticFriction(),
                BodyAComponent->Material.FrictionCombinationType,
                BodyBComponent->Material.FrictionCombinationType
            );
            const auto DynamicFrictionCoef = CombinePhysicsProperties(
                BodyAComponent->Material.GetDynamicFriction(),
                BodyBComponent->Material.GetDynamicFriction(),
                BodyAComponent->Material.FrictionCombinationType,
                BodyBComponent->Material.FrictionCombinationType
            );

            RelativeVelocity = (
                (BodyB->LinearVelocity + VecFromCenterOfMassB * BodyB->AngularVelocity)
                - (BodyA->LinearVelocity + VecFromCenterOfMassA * BodyA->AngularVelocity)
            );
            // Tangent must be in similar direction than velocity
            const auto TangentialVector = (RelativeVelocity - (Normal * RelativeVelocity.Scalar(Normal))).GetNormalized();
            const auto RelativeVelocityOnTangent = Maths::ProjectOnUnitVector(
                RelativeVelocity,
                TangentialVector
            );

            // (-(1 + e)((Va - Vb) ⋅ -t)) / (1 / ma + 1 / mb + ((ra ⋅ n)² / Ia) + ((rb ⋅ n)² / Ib))
            // where e is the friction coef
            auto jT = (RelativeVelocityOnTangent * -1)
                      / (
                          BodyA->InvMass + BodyB->InvMass
                           + (AngularAImpulseDenom)
                           + (AngularBImpulseDenom)
                      );

            if (jT < 0.000001f)
                jT = 0;

            // Coulomb's Law
            typename RigidBodyComponentT::PhysicsVectorT FrictionImpulse;
            if (std::abs(jT) < jN * StaticFrictionCoef) {
                // static friction
                FrictionImpulse = TangentialVector * jT;
            } else {
                // dynamic friction
                FrictionImpulse = TangentialVector * -jN * DynamicFrictionCoef;
            }
            BodyA->LinearVelocity = BodyA->LinearVelocity - FrictionImpulse * (BodyA->InvMass);
            BodyB->LinearVelocity = BodyB->LinearVelocity + FrictionImpulse * (BodyB->InvMass);

            BodyA->AngularVelocity -= (VecFromCenterOfMassA ^ FrictionImpulse) * BodyA->InvMomentOfInertiaForZ;
            BodyB->AngularVelocity += (VecFromCenterOfMassB ^ FrictionImpulse) * BodyB->InvMomentOfInertiaForZ;
        }

        template <typename GeometricType, size_t Dimensions>
        Maths::Collisions::CollisionManifold<GeometricType, Dimensions> CollideCachedEntities(
            const CachedEntity& CachedEntityA,
            const CachedEntity& CachedEntityB
        ) const
        {
            auto RigidBodyA = CachedEntityA.Body->GetRigidBody();
            auto RigidBodyB = CachedEntityB.Body->GetRigidBody();

            auto CastedRigidBodyA = dynamic_cast<const Engine::Components::Physics::IRectangleGeometricRigidBody2D<GeometricType>*>(RigidBodyA);
            auto CastedRigidBodyB = dynamic_cast<const Engine::Components::Physics::IRectangleGeometricRigidBody2D<GeometricType>*>(RigidBodyB);
            auto ShapeA = dynamic_cast<const Maths::IShape*>(RigidBodyA->GetBoundingBox());
            auto ShapeB = dynamic_cast<const Maths::IShape*>(RigidBodyB->GetBoundingBox());
            if (CastedRigidBodyA && CastedRigidBodyB && ShapeA && ShapeB) {
                if (
                    ShapeA->getType()->isInstanceOrChildOf(Maths::Rectangle2D<GeometricType>::getClassType())
                    && ShapeB->getType()->isInstanceOrChildOf(Maths::Rectangle2D<GeometricType>::getClassType())
                ) {
                    return Maths::Collisions::SAT::CollideRectangles<GeometricType, Dimensions>(
                        CastedRigidBodyA->GetNormals(),
                        CastedRigidBodyB->GetNormals(),
                        CastedRigidBodyA->GetVertices(),
                        CastedRigidBodyB->GetVertices()
                    );
                }
            }

            throw std::runtime_error("No matching function found");
        }

        template <typename GeometricType, size_t Dimensions>
        bool AreCachedEntitiesLikelyToCollide(
            const Maths::Vector<GeometricType, Dimensions>& Axis,
            const CachedEntity& EntityA,
            const CachedEntity& EntityB
        ) const
        {
            auto RigidBodyA = EntityA.Body->GetRigidBody();
            auto RigidBodyB = EntityB.Body->GetRigidBody();

            auto CastedRigidBodyA = dynamic_cast<const Engine::Components::Physics::IRectangleGeometricRigidBody2D<GeometricType>*>(RigidBodyA);
            auto CastedRigidBodyB = dynamic_cast<const Engine::Components::Physics::IRectangleGeometricRigidBody2D<GeometricType>*>(RigidBodyB);
            if (CastedRigidBodyA && CastedRigidBodyB) {
                return Maths::Collisions::SAP::LikelyToCollide<GeometricType, Dimensions>(
                    Axis,
                    CastedRigidBodyA->GetVertices(),
                    CastedRigidBodyB->GetVertices()
                );
            }

            return false;
        }

        template <typename GeometricType, size_t Dimensions>
        Maths::Collisions::CollisionManifold<GeometricType, Dimensions> CollideBoundingBoxes(
            const Maths::Vector<GeometricType, Dimensions>& ScaleA,
            const Maths::Vector<GeometricType, Dimensions>& ScaleB,
            const GeometricType& RotationDegreesA,
            const GeometricType& RotationDegreesB,
            const Maths::Point2D<GeometricType>& OnPosA,
            const Maths::Point2D<GeometricType>& OnPosB,
            const Maths::IBoundingBox2D<GeometricType>* ShapeA,
            const Maths::IBoundingBox2D<GeometricType>* ShapeB
        ) const
        {
            auto CastedShapeA = dynamic_cast<const Maths::IShape*>(ShapeA);
            auto CastedShapeB = dynamic_cast<const Maths::IShape*>(ShapeB);
            if (CastedShapeA && CastedShapeB) {
                return Maths::Collisions::SAT::Collide(
                    ScaleA,
                    ScaleB,
                    RotationDegreesA,
                    RotationDegreesB,
                    OnPosA,
                    OnPosB,
                    static_cast<const Maths::IShape*>(CastedShapeA),
                    static_cast<const Maths::IShape*>(CastedShapeB)
                );
            }

            throw std::runtime_error("No matching function found.");
        }

        template <typename GeometricType, size_t Dimensions>
        bool AreBoundingBoxLikelyToCollide(
            const Maths::Vector<GeometricType, Dimensions>& Axis,
            const Maths::Vector<GeometricType, Dimensions>& ScaleA,
            const Maths::Vector<GeometricType, Dimensions>& ScaleB,
            const GeometricType& RotationDegreesA,
            const GeometricType& RotationDegreesB,
            const Maths::Point2D<GeometricType>& OnPosA,
            const Maths::Point2D<GeometricType>& OnPosB,
            const Maths::IBoundingBox2D<GeometricType>* ShapeA,
            const Maths::IBoundingBox2D<GeometricType>* ShapeB
        ) const
        {
            auto CastedShapeA = dynamic_cast<const Maths::IShape*>(ShapeA);
            auto CastedShapeB = dynamic_cast<const Maths::IShape*>(ShapeB);
            if (CastedShapeA && CastedShapeB) {
                return Maths::Collisions::SAP::LikelyToCollide<GeometricType, Dimensions>(
                    Axis,
                    ScaleA,
                    ScaleB,
                    RotationDegreesA,
                    RotationDegreesB,
                    OnPosA,
                    OnPosB,
                    static_cast<const Maths::IShape*>(CastedShapeA),
                    static_cast<const Maths::IShape*>(CastedShapeB)
                );
            }

            throw std::runtime_error("No matching function found.");
        }
    };
}
