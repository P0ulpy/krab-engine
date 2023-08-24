#pragma once

#include "../../Maths/Shape.hpp"
#include "../../ECS/Component/Component.hpp"
#include "Transform.hpp"
#include "../../Maths/Collisions.hpp"
#include "../Application/EngineApplication.hpp"
#include "../Camera/Camera.hpp"
#include "../ScenesSystem/ScenesSystem.hpp"
#include <vector>
#include <SFML/Graphics.hpp>

namespace KrabEngine::Components::Physics
{
    // All distances are in centimeters
    // All forces are in m/s²
    // All masses are in kg

    template <typename T>
    static constexpr Maths::Vector2D<T> GetUpVector2D()
    {
        return Maths::Vector2D<T>::GetUnitVectorOnAxis(1) * -1;
    }
    template <typename T>
    static constexpr Maths::Vector2D<T> GetDownVector2D()
    {
        return Maths::Vector2D<T>::GetUnitVectorOnAxis(1);
    }

    template <typename T>
    static constexpr Maths::Vector2D<T> GetLeftChildVector2D()
    {
        return Maths::Vector2D<T>::GetUnitVectorOnAxis(0) * -1;
    }

    template <typename T>
    static constexpr Maths::Vector2D<T> GetRightChildVector2D()
    {
        return Maths::Vector2D<T>::GetUnitVectorOnAxis(0);
    }

    // If 2 different combination types, then priority to the lowest vaue
    enum struct PhysicsPropertiesCombinationType {
            AVERAGE = 0,
            MINIMUM,
            MAXIMUM,
            MULTIPLY,
            SQUARED
    };

    template <size_t Dimensions, typename GeometricT, typename PhysicsT>
    class PhysicsMaterial
    {
        // 0 - 1, restitution
        PhysicsT m_Restitution = 0.f;
        // 0 - 1, friction on surface when objects are at "rest"
        PhysicsT m_StaticFriction = 0.5f;
        // 0 - 1, friction on surface when objects are moving
        PhysicsT m_DynamicFriction = 0.5f;

    public:
        PhysicsPropertiesCombinationType RestitutionCombinationType = PhysicsPropertiesCombinationType::AVERAGE;
        PhysicsPropertiesCombinationType FrictionCombinationType = PhysicsPropertiesCombinationType::AVERAGE;

        PhysicsMaterial() = default;

        inline const PhysicsT& GetRestitution() const { return m_Restitution; }
        inline const PhysicsT& GetStaticFriction() const { return m_StaticFriction; }
        inline const PhysicsT& GetDynamicFriction() const { return m_DynamicFriction; }

        inline void SetRestitution(PhysicsT Restitution) const { m_Restitution = std::clamp(Restitution, PhysicsT(0), PhysicsT(1)); }
        inline void SetStaticFriction(PhysicsT StaticFriction) const { m_StaticFriction = std::clamp(StaticFriction, PhysicsT(0), PhysicsT(1)); }
        inline void SetDynamicFriction(PhysicsT DynamicFriction) const { m_DynamicFriction = std::clamp(DynamicFriction, PhysicsT(0), PhysicsT(1)); }
    };

    template <size_t Dimensions, typename GeometricT, typename PhysicsT>
    struct IRigidBody
    {
        Maths::Vector2D<PhysicsT> LinearVelocity;
        // radians/s
        PhysicsT AngularVelocity;
        Maths::Vector2D<PhysicsT> Force;
        PhysicsT Torque = 0.f;
        // kg, if 0 then infinite mass
        PhysicsT Mass = 1.f;
        PhysicsT InvMass = 1.f;
        // m4
        PhysicsT InvMomentOfInertiaForZ;

        virtual void ComputePhysicsProperties(const Maths::Vector2D<GeometricT>& Scale) = 0;
        virtual Maths::Point<GeometricT, Dimensions> GetCenterOfMass(
            const Maths::Vector2D<GeometricT>& Scale,
            const GeometricT& RotationDegrees,
            const Maths::Point2D<GeometricT>& OnPos
        ) = 0;
        virtual void HandleComputeCache(
            const GeometricT& RotationDegrees,
            const Maths::Vector2D<GeometricT>& Scale,
            const Maths::Point2D<GeometricT>& OnPos,
            bool Force
        ) = 0;

        virtual GeometricT GetMinProjectedOnAxis(const Maths::Vector2D<GeometricT>& Axis) const = 0;
        virtual ~IRigidBody() = default;

        bool IsCacheComputed = false;
    };

    template <typename GeometricT, typename PhysicsT>
    struct IRigidBody2D : public IRigidBody<2, GeometricT, PhysicsT>
    {
        using IBoundingBox2D = Maths::IBoundingBox2D<GeometricT>;
        virtual IBoundingBox2D* GetBoundingBox() = 0;
        virtual void AddForce(
            const Maths::Vector2D<PhysicsT>& Force,
            const Maths::Vector2D<GeometricT>& DistanceFromMass
        ) = 0;
    };

    // enable using RigidBody cached geometric values
    template <typename GeometricT>
    struct IRectangleGeometricRigidBody2D
    {
        virtual std::vector<Maths::Point2D<GeometricT>> GetVertices() const = 0;
        virtual std::vector<Maths::Vector2D<GeometricT>> GetEdges() const = 0;
        virtual std::vector<Maths::Vector2D<GeometricT>> GetNormals() const = 0;
        virtual ~IRectangleGeometricRigidBody2D() = default;
    };

    template <typename GeometricT, typename PhysicsT, template <typename GT> typename BoundingBoxT>
    struct RigidBody2D : public IRigidBody2D<GeometricT, PhysicsT>
    {
        static_assert(std::is_convertible<BoundingBoxT<GeometricT>*, typename IRigidBody2D<GeometricT, PhysicsT>::IBoundingBox2D*>::value, "BoundingBox must inherit IBoundingBox2D");

        BoundingBoxT<GeometricT> BoundingBox;

        IRigidBody2D<GeometricT, PhysicsT>::IBoundingBox2D* GetBoundingBox() override
        {
            return static_cast<IRigidBody2D<GeometricT, PhysicsT>::IBoundingBox2D*>(&BoundingBox);
        }
    };

    template <typename GeometricT, typename PhysicsT>
    struct RectangleRigidBody2D : public RigidBody2D<GeometricT, PhysicsT, Maths::RectangleBoundingBox2D>, public IRectangleGeometricRigidBody2D<GeometricT>
    {
        Maths::Point2D<GeometricT> GetCenterOfMass(
            const Maths::Vector2D<GeometricT>& Scale,
            const GeometricT& RotationDegrees,
            const Maths::Point2D<GeometricT>& OnPos
        ) override
        {
            return Maths::Point2D<GeometricT>(GeometricT(0), GeometricT(0)) + Maths::Vector2D<GeometricT>{OnPos.Values};
        }

        void ComputePhysicsProperties(const Maths::Vector2D<GeometricT>& Scale)
        {
            // meters
            const auto w = this->BoundingBox.Width * Scale.GetX() / 100.f;
            const auto h = this->BoundingBox.Height * Scale.GetY() / 100.f;
            if (this->Mass == 0) {
                this->InvMass = 0;
                this->InvMomentOfInertiaForZ = 0;
            } else {
                this->InvMass = 1 / this->Mass;
                this->InvMomentOfInertiaForZ =  1 / (this->Mass * (w * w + h * h) / 12.f);
            }
        }

        void AddForce(
            const Maths::Vector2D<PhysicsT>& Force,
            const Maths::Vector2D<GeometricT>& DistanceFromMass
        )
        {
            this->Force += Force;
            // in meters
            this->Torque += (DistanceFromMass ^ this->Force);
        }

        void HandleComputeCache(
            const GeometricT& RotationDegrees,
            const Maths::Vector2D<GeometricT>& Scale,
            const Maths::Point2D<GeometricT>& OnPos,
            bool Force
        ) override
        {
            if (!Force && this->IsCacheComputed) return;

            this->IsCacheComputed = true;
            CachedVertices = this->BoundingBox.GetVertices(RotationDegrees, Scale, OnPos);
            CachedEdges = this->BoundingBox.GetEdges(CachedVertices);
            CachedNormals = this->BoundingBox.GetNormals(CachedEdges);
        }

        std::vector<Maths::Point2D<GeometricT>> GetVertices() const override
        {
            return CachedVertices;
        }

        std::vector<Maths::Vector2D<GeometricT>> GetEdges() const override
        {
            return CachedEdges;
        }

        std::vector<Maths::Vector2D<GeometricT>> GetNormals() const override
        {
            return CachedNormals;
        }

        GeometricT GetMinProjectedOnAxis(const Maths::Vector2D<GeometricT>& Axis) const override
        {
            GeometricT Min;
            Maths::Point2D<GeometricT> PointMin;
            Maths::Collisions::GetMinProjectedValues(
                Axis,
                CachedVertices,
                Min,
                PointMin
            );

            return Min;
        }
    private:
        std::vector<Maths::Point2D<GeometricT>> CachedVertices;
        std::vector<Maths::Vector2D<GeometricT>> CachedEdges;
        std::vector<Maths::Vector2D<GeometricT>> CachedNormals;
    };

    using RectangleRigidBody2Dd = RectangleRigidBody2D<double, double>;

    template <typename GT, typename PT, bool DebugShapes, bool DebugPoints>
    class RigidBody2DComponent : public KrabEngine::Component
    {
    public:
        DECLARE_CLASS_TYPE(RigidBody2DComponent, KrabEngine::Component)

        static constexpr size_t Dimensions = 2;
        using RequiredComponents = ComponentRequirements<KrabEngine::Components::Transform>;
        using MaterialT = PhysicsMaterial<Dimensions, GT, PT>;
        using GeometricT = GT;
        using PhysicsT = PT;
        using PhysicsPointT = Maths::Point<PhysicsT, Dimensions>;
        using GeometricPointT = Maths::Point<GeometricT, Dimensions>;
        using PhysicsVectorT = Maths::Vector<PhysicsT, Dimensions>;
        using GeometricVectorT = Maths::Vector<GeometricT, Dimensions>;
        using IRigidBodyT = IRigidBody2D<GeometricT, PhysicsT>;

        static constexpr PT Gravity = PT(9.80665f);
        static PhysicsVectorT GravityDirection;
        static constexpr PT LinearDrag = PT(0.05f);
        static constexpr PT AngularDrag = PT(0.5f);

        MaterialT Material;
        float GravityScale = 1.f;
        bool HasCollisions = true;
        bool IsStatic = false;
        bool CanRotate = true;

        IRigidBodyT* GetRigidBody()
        {
            return m_RigidBody;
        }

        void SetRigidBody(IRigidBodyT* Body)
        {
            m_RigidBody = Body;
        }

        template <bool DShapes = DebugShapes, bool DPoints = DebugPoints, std::enable_if_t<(DShapes || DPoints), bool> = false>
        void OnRender(sf::RenderTarget& RenderTarget) const
        {
            auto ActiveCamera = GetScene()->GetActiveCamera();
            auto PixelSize = 1.f;
            if (dynamic_cast<KrabEngine::Camera::Camera2D<GeometricT>*>(ActiveCamera))
            {
                PixelSize = PixelSize / static_cast<KrabEngine::Camera::Camera2D<GeometricT>*>(ActiveCamera)->PixelCentimeterRatio;
            }

            auto EntityTransform = GetEntityTransform();

            auto BoundingBox = m_RigidBody->GetBoundingBox();

            const auto RectangleBoundingBox = dynamic_cast<Maths::RectangleBoundingBox2D<GeometricT>*>(BoundingBox);
            if (RectangleBoundingBox) {
                if constexpr (DShapes)
                {
                    sf::RectangleShape Rect;
                    const auto ScaledWidth = RectangleBoundingBox->Width * EntityTransform->Scale.GetX();
                    const auto ScaledHeight = RectangleBoundingBox->Height * EntityTransform->Scale.GetY();
                    Rect.setOrigin(
                        ScaledWidth / 2,
                        ScaledHeight / 2
                    );
                    Rect.setSize({
                                     (float) (ScaledWidth),
                                     (float) (ScaledHeight)
                                 });
                    const auto FinalPos = EntityTransform->Pos + (GeometricVectorT(
                        RectangleBoundingBox->Position.GetX(),
                        RectangleBoundingBox->Position.GetY()
                    ) * EntityTransform->Scale);
                    Rect.setPosition({
                                         (float) (FinalPos.GetX()),
                                         (float) (FinalPos.GetY()),
                                     });
                    Rect.setRotation(RectangleBoundingBox->Angle + EntityTransform->Angle);
                    Rect.setFillColor(sf::Color::Transparent);
                    Rect.setOutlineColor(sf::Color::White);
                    Rect.setOutlineThickness(PixelSize);
                    RenderTarget.draw(Rect);
                }

                if constexpr (DPoints)
                {
                    const auto Vertices = RectangleBoundingBox->GetVertices(
                        EntityTransform->Angle,
                        EntityTransform->Scale,
                        EntityTransform->Pos
                    );
                    // Draw vertices manually to verify previous drawing
                    for (size_t i = 0; i < Vertices.size(); i++)
                    {
                        const auto& Vertex = Vertices[i];
                        sf::CircleShape Point;
                        // 2 mm
                        Point.setRadius(PixelSize * 2);
                        Point.setOrigin(PixelSize * 2, PixelSize * 2);
                        Point.setFillColor(sf::Color::Red);
                        Point.setPosition({
                            (float) Vertex.GetX(),
                            (float) Vertex.GetY()
                        });
                        RenderTarget.draw(Point);
                        sf::Text PositionText;
                        PositionText.setFillColor(sf::Color::Red);
                        std::ostringstream PosString;
                        PosString << std::round(Vertex.GetX()) << " , " << std::round(Vertex.GetY());
                        PositionText.setString(PosString.str());
                        sf::Font Lato;
                        Lato.loadFromFile("Assets/Lato/Lato-Regular.ttf");
                        PositionText.setFont(Lato);
                        PositionText.setCharacterSize(12);
                        PositionText.setPosition({
                             (float) Vertex.GetX(),
                             (float) Vertex.GetY() - (PixelSize * 10.f)
                         });

                        const int HeightModifier = ((((int) i) / 2) % 2);
                        PositionText.setOrigin(
                            // small trick to get pos for text
                            PositionText.getGlobalBounds().width * (float) ((((int) i + 3) / 2) % 2),
                            PositionText.getGlobalBounds().height * (float) (2 * (1 - HeightModifier) - 1)
                        );
                        PositionText.setScale({PixelSize, PixelSize});
                        RenderTarget.draw(PositionText);
                    }
                }
            }

            const auto CircleBoundingBox = dynamic_cast<Maths::CircleBoundingBox2D<GeometricT>*>(BoundingBox);
            if (CircleBoundingBox) {
                if constexpr (DShapes)
                {
                    sf::CircleShape Circle;
                    Circle.setRadius(CircleBoundingBox->Radius);
                    const auto FinalPos = EntityTransform->Pos + (GeometricVectorT(
                        RectangleBoundingBox->Position.GetX(),
                        RectangleBoundingBox->Position.GetY()
                    ) * EntityTransform->Scale);
                    Circle.setPosition({
                                         (float) (FinalPos.GetX()),
                                         (float) (FinalPos.GetY()),
                                     });
                    Circle.setRotation(RectangleBoundingBox->Angle + EntityTransform->Angle);
                    Circle.setFillColor(sf::Color::Transparent);
                    Circle.setOutlineColor(sf::Color::White);
                    Circle.setOutlineThickness(PixelSize);
                    RenderTarget.draw(Circle);
                }

                if constexpr (DPoints)
                {
                    const auto& Vertex = CircleBoundingBox->Position + GeometricVectorT{EntityTransform->Pos.Values};
                    sf::CircleShape Point;
                    // 2 mm
                    Point.setRadius(PixelSize * 2);
                    Point.setOrigin(PixelSize * 2, PixelSize * 2);
                    Point.setFillColor(sf::Color::Red);
                    Point.setPosition({
                                          (float) Vertex.GetX(),
                                          (float) Vertex.GetY()
                                      });
                    RenderTarget.draw(Point);
                    sf::Text PositionText;
                    PositionText.setFillColor(sf::Color::Red);
                    std::ostringstream PosString;
                    PosString << std::round(Vertex.GetX()) << " , " << std::round(Vertex.GetY());
                    PositionText.setString(PosString.str());
                    sf::Font Lato;
                    Lato.loadFromFile("Assets/Lato/Lato-Regular.ttf");
                    PositionText.setFont(Lato);
                    PositionText.setCharacterSize(12);
                    PositionText.setPosition({
                         (float) Vertex.GetX(),
                         (float) Vertex.GetY() - (PixelSize * 10.f)
                     });

                    PositionText.setOrigin(
                        // small trick to get pos for text
                        PositionText.getGlobalBounds().width / 2,
                        PositionText.getGlobalBounds().height / 2
                    );
                    PositionText.setScale({PixelSize, PixelSize});
                    RenderTarget.draw(PositionText);
                }
            }
        }

        GeometricT ComputeMaxDistanceFromTransform() const
        {
            GeometricT MaxDistance = 0;
            for (const auto& Body : this->m_RigidBody)
            {
                const auto& BoundingBox = Body->GetBoundingBox();
                const auto BodyMaxDistance = BoundingBox->ComputeMaxDistanceFromPosition() + GeometricVectorT(
                    BoundingBox->Position.GetX(),
                    BoundingBox->Position.GetY()
                ).GetLength();
                if (MaxDistance < BodyMaxDistance)
                {
                    MaxDistance = BodyMaxDistance;
                }
            }
            return MaxDistance;
        }

        // optimisation
        void SimulateRigidBody(
            const float& StepMs,
            KrabEngine::Components::Transform* Transform
        )
        {
            ComputeRigidBodyPhysicsProperties(
                Transform->Scale,
                m_RigidBody
            );
            ApplyRigidBodyToEntity(
                StepMs,
                Transform,
                m_RigidBody
            );
        }

        void SimulateRigidBody(const float& StepMs)
        {
            auto Transform = GetEntityTransform();
            SimulateRigidBody(StepMs, Transform);
        }

        [[nodiscard]] KrabEngine::Components::Transform* GetEntityTransform()
        {
            return this->GetEntity().template GetComponent<KrabEngine::Components::Transform>();
        }

        [[nodiscard]] const KrabEngine::Components::Transform* GetEntityTransform() const
        {
            return this->GetEntity().template GetComponent<KrabEngine::Components::Transform>();
        }

        void HandleRecomputeCachedProperties(bool Force = false)
        {
            const auto Transform = GetEntityTransform();
            m_RigidBody->HandleComputeCache(
                Transform->Angle,
                Transform->Scale,
                Transform->Pos,
                Force
            );
        }

        void SetOnCollisionCallback(const std::function<bool(RigidBody2DComponent*, const GeometricVectorT&)>& Callback)
        {
            m_OnCollisionCallback = Callback;
        }

        bool OnCollide(RigidBody2DComponent* WithComponent, const GeometricVectorT& CollisionNormal)
        {
            bool BlockCollide = false;
            if(m_OnCollisionCallback)
            {
                if (m_OnCollisionCallback(WithComponent, CollisionNormal))
                {
                    BlockCollide = true;
                }
            }

            return BlockCollide;
        }

        void OnDestroy()
        {
            delete m_RigidBody;
        }

    private:
        // @todo can be useful to have multiple rigidbodies with constraints
        IRigidBodyT* m_RigidBody;
        std::function<bool(RigidBody2DComponent*, const GeometricVectorT& CollisionNormal)> m_OnCollisionCallback;

        void ComputeRigidBodyPhysicsProperties(
            const Maths::Vector2D<GeometricT>& Scale,
            IRigidBodyT* RigidBody
        )
        {
            if (this->IsStatic) {
                RigidBody->Mass = PhysicsT(0);
            } else if (RigidBody->Mass == 0) {
                RigidBody->Mass = PhysicsT(1);
            }
            RigidBody->ComputePhysicsProperties(Scale);
        }

        void ApplyRigidBodyToEntity(
            const float& StepMs,
            KrabEngine::Components::Transform* EntityTransform,
            IRigidBodyT* RigidBody
        )
        {
            if (this->IsStatic) {
                RigidBody->LinearVelocity = PhysicsVectorT(PhysicsT(0.f), PhysicsT(0.f));
                RigidBody->AngularVelocity = 0;
                return;
            }

            RigidBody->IsCacheComputed = false;
            const auto GravityVec = GravityDirection * GravityScale * Gravity;
            const PhysicsVectorT linearAcceleration = PhysicsVectorT(
                RigidBody->Force.GetX() * RigidBody->InvMass,
                RigidBody->Force.GetY() * RigidBody->InvMass
            );
            // all forces are in m/s
            const auto StepMsMultiplier = (StepMs / 1000);
            RigidBody->LinearVelocity.GetX() += (linearAcceleration.GetX() + GravityVec.GetX()) * StepMsMultiplier;
            RigidBody->LinearVelocity.GetY() += (linearAcceleration.GetY() + GravityVec.GetY()) * StepMsMultiplier;

            if (CanRotate) {
                const auto angularAcceleration = RigidBody->Torque * RigidBody->InvMomentOfInertiaForZ;
                EntityTransform->Angle += Maths::Angles::RadToDeg(RigidBody->AngularVelocity * StepMsMultiplier);
                if (EntityTransform->Angle >= 360.f) {
                    EntityTransform->Angle -= 360.f;
                }
                RigidBody->AngularVelocity += angularAcceleration * StepMsMultiplier;
            }

            // in centimeters
            EntityTransform->Pos.GetX() += (RigidBody->LinearVelocity.GetX() * 100.f) * StepMsMultiplier;
            EntityTransform->Pos.GetY() += (RigidBody->LinearVelocity.GetY() * 100.f) * StepMsMultiplier;

            // apply drag to simulate air friction
            RigidBody->LinearVelocity = RigidBody->LinearVelocity * std::clamp(PhysicsT(1.f) - LinearDrag * StepMsMultiplier, PhysicsT(0.f), PhysicsT(1.f));
            RigidBody->AngularVelocity *= std::clamp(PhysicsT(1.f) - AngularDrag * StepMsMultiplier, PhysicsT(0.f), PhysicsT(1.f));

            // reset applied force each time it is applied
            RigidBody->Force = PhysicsVectorT{};
            RigidBody->Torque = 0;
        }
    };

    template <typename GT, typename PT, bool DebugShapes, bool DebugPoints>
    RigidBody2DComponent<GT, PT, DebugShapes, DebugPoints>::PhysicsVectorT RigidBody2DComponent<GT, PT, DebugShapes, DebugPoints>::GravityDirection = GetDownVector2D<PhysicsT>();

    using RigidBody2DdComponent = RigidBody2DComponent<double, double, true, false>;
    using RigidBody2DdComponentDebug = RigidBody2DComponent<double, double, true, true>;
}