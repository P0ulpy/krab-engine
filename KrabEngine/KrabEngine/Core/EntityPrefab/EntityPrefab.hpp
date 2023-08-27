//
// Created by Flo on 27/02/2023.
//

#pragma once

#include "../../ECS/Entity/Entity.hpp"
#include "../../ECS/Scene/Scene.hpp"

namespace KrabEngine
{
    /**
     * @brief A prefab is a template for an entity.
     * It is used to create entities with a specific set of components.
     * Prefabs inherited class should never be inherited, in this case the TComponents will never be instantiated.
     * The lifetime of the prefab class instance is only used to instantiate the entity, so after the instantiation, the prefab class instance is destroyed.
     * Prefabs are should never be instantiated directly, but through the Scene::InstantiatePrefab() method.
     * @tparam TComponents The components to add to the entity.
     */
    template <class... TComponents>
    class EntityPrefab
    {
    public:
        ~EntityPrefab() = default;

        [[nodiscard]] Entity Instantiate(Scene* scene);
        virtual void Init(Entity& entity) = 0;

    private:
        template<class TComponent, class... TComponentsToInstantiate>
        void InstantiateComponents(Entity& entity);

        friend class Scene;
    };

    template<class... TComponents>
    Entity EntityPrefab<TComponents...>::Instantiate(Scene* scene)
    {
        Entity entity = scene->CreateEntity();
        InstantiateComponents<TComponents...>(entity);
        Init(entity);

        return entity;
    }

    template<class... TComponents>
    template<class TComponent, class... TComponentsToInstantiate>
    void EntityPrefab<TComponents...>::InstantiateComponents(Entity &entity)
    {
        if constexpr (sizeof...(TComponentsToInstantiate) > 0)
        {
            entity.AddComponent<TComponent>();
            InstantiateComponents<TComponentsToInstantiate...>(entity);
        }
    }

} // KrabEngine
