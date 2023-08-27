//
// Created by Flo on 25/11/2022.
//

#pragma once

#include "SFML/Graphics/RenderTarget.hpp"

#include "../../Core/Time.hpp"

#include "../Entity/Entity.hpp"
#include "../EntitiesRegistry/EntitiesRegistry.hpp"
#include "../../Core/Camera/Camera.hpp"

namespace KrabEngine
{
    namespace Physics {
        class IPhysicsSimulator;
    }
    /**
     * @brief A scene is a collection of entities.
     * It is used to group entities together.
     * it no store directly the entities, but a registry that store the entities.
     * The scene is responsible for the lifetime of the entities.
     */
    class Scene
    {
    public:
        Scene() = default;
        ~Scene();

        void OnStart();
        void OnUpdate(Timestep DeltaTime);
        void OnStop();

        void Clear();

        void RenderScene(sf::RenderTarget& renderTarget);

        Entity CreateEntity();

        template <class TPrefab>
        Entity InstantiatePrefab();

        void DestroyEntity(const Entity& entity);
        void DestroyEntity(const EntityHandle& entity);

        template<class T>
        T* GetComponentOf(const EntityHandle& entityHandle);

        template<class T>
        T* AddComponentTo(const EntityHandle& entityHandle);

        template<class T>
        bool HasComponentIn(const EntityHandle& entityHandle);

        template<class T>
        void RemoveComponentOf(const EntityHandle& entityHandle);

        void ApplyCleanup();

        inline KrabEngine::Camera::ICamera* GetActiveCamera() { return m_ActiveCamera; }
        inline void SetActiveCamera(KrabEngine::Camera::ICamera* Camera) { m_ActiveCamera = Camera; }
        void AddPhysicsSimulator(KrabEngine::Physics::IPhysicsSimulator* Simulator);
        void RemovePhysicsSimulator(KrabEngine::Physics::IPhysicsSimulator *Simulator);

        // @todo maybe not pertinent
        EntitiesRegistry* GetRegistry() {return &m_registry;}
    private:
        EntitiesRegistry m_registry = EntitiesRegistry(this);
        KrabEngine::Camera::ICamera* m_ActiveCamera = nullptr;
        std::unordered_map<RTTI::ClassType*, KrabEngine::Physics::IPhysicsSimulator*> m_PhysicsSimulators;
    };

    // Scene

    template<class TPrefab>
    Entity Scene::InstantiatePrefab()
    {
        // TODO : Assert TPrefab inherit from EntityPrefab<>

        TPrefab prefab;
        return prefab.Instantiate(this);
    }

    template<class T>
    T *Scene::GetComponentOf(const EntityHandle &entityHandle)
    {
        return m_registry.GetComponentOf<T>(entityHandle);
    }

    template<class T>
    T *Scene::AddComponentTo(const EntityHandle &entityHandle)
    {
        T* component = m_registry.AddComponentTo<T>(entityHandle);
        if(component)
            component->m_scene = this;
        return component;
    }

    template<class T>
    bool Scene::HasComponentIn(const EntityHandle &entityHandle)
    {
        return m_registry.HasComponent<T>(entityHandle);
    }

    template<class T>
    void Scene::RemoveComponentOf(const EntityHandle &entityHandle)
    {
        m_registry.RemoveComponentOf<T>(entityHandle);
    }

    // Entity

    template<class T>
    T* Entity::AddComponent()
    {
        return m_scene->AddComponentTo<T>(m_handle);
    }

    template<class T>
    T* Entity::GetComponent()
    {
        return m_scene->GetComponentOf<T>(m_handle);
    }

    template<class T>
    const T* Entity::GetComponent() const
    {
        return m_scene->GetComponentOf<T>(m_handle);
    }

    template<class T>
    bool Entity::HasComponent() const
    {
        return m_scene->HasComponentIn<T>(m_handle);
    }

    template<class T>
    void Entity::RemoveComponent()
    {
        m_scene->RemoveComponentOf<T>(m_handle);
    }

} // KrabEngine
