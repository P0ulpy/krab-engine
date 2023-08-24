//
// Created by Flo on 26/11/2022.
//

#pragma once

#include <unordered_map>
#include <map>
#include <stack>

#include "SFML/Graphics/RenderTarget.hpp"

#include "../ComponentSystem/ComponentSystem.hpp"
#include "../../RTTI/ClassType.hpp"

namespace KrabEngine
{
    class Scene;
    
    /**
     * @brief Entities registry is the core interface for our ESC system
     * it manages Components association with Entities and ComponentSystems.
     */
    class EntitiesRegistry
    {
    public:
        EntitiesRegistry() = delete;
        EntitiesRegistry(Scene* Scene) : m_Scene(Scene) {}

        EntityHandle CreateEntity();

        void DestroyEntity(EntityHandle handle);

        template<class T>
        T* AddComponentTo(EntityHandle entityHandle);

        template<class T>
        T* AddComponentIfNotPresentTo(EntityHandle entityHandle);

        template<class ...T>
        void AddComponentsTo(EntityHandle entityHandle);

        template<class ...T>
        void AddComponentsIfNotPresentTo(EntityHandle entityHandle);

        template<class T>
        T* GetComponentOf(EntityHandle entityHandle);

        template<class T>
        bool HasComponent(EntityHandle entityHandle);

        template<class T>
        void RemoveComponentOf(EntityHandle entityHandle);

        template<class T>
        void View(ComponentSystem<T>::ViewCallback callback);

        void AwakeAll();
        void StartAll();
        void UpdateAllUpdatable(const float& deltaTime);
        std::unordered_map<RTTI::ClassType*, IComponentSystem*> GetAllRenderableSystems();

        void ApplyCleanup();

        template<class TSystem, class T>
        void AddSystem();

        template<class T>
        ComponentSystem<T>* GetSystem();

        void DestroyAll();

    private:
        bool m_markedForDestruction = false;

        std::unordered_map<RTTI::ClassType*, std::unique_ptr<IComponentSystem>> m_componentSystems;

        std::unordered_map<RTTI::ClassType*, IComponentSystem*> m_updatableSystems;
        std::unordered_map<RTTI::ClassType*, IComponentSystem*> m_renderableSystems;

        void View(IComponentSystem::ViewCallback callback);

        Scene* m_Scene = nullptr;
    };

} // Engine

#include "EntitiesRegistry.tpp"
