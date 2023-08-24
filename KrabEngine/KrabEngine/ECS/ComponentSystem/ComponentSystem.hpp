//
// Created by Flo on 18/01/2023.
//

#pragma once

#include "IComponentSystem.hpp"

#include "../../Core/Logger/Logger.hpp"
#include <unordered_map>
#include <list>

namespace KrabEngine
{
    // TODO : use a custom memory pool instead of std::unordered_map to store components contiguously

    /**
     * @brief A ComponentSystem is a container of components of the same type
     * @tparam TComponent The type of the component to store it should be derived from Component
     */
    template <class TComponent>
    class ComponentSystem : public IComponentSystem
    {
        static_assert(std::is_base_of_v<Component, TComponent>, "Invalid TComponent type, a component must be derived from KrabEngine::Component");

    public:
        ~ComponentSystem() override;

        std::unordered_map<EntityHandle, TComponent> components {};
        std::list<EntityHandle> componentsToBeCleaned {};
        bool NeedCleanup() override { return !componentsToBeCleaned.empty(); }

        Component* Add(EntityHandle entityHandle, Scene* scene) override;
        void RemoveAfter(EntityHandle entityHandle) override;
        void Remove(EntityHandle entityHandle) override;
        void Remove(EntityHandle entityHandle, typename std::unordered_map<EntityHandle, TComponent>::iterator& removedComponentIt);
        bool Has(EntityHandle entityHandle) override;
        Component* Get(EntityHandle entityHandle) override;
        TComponent* GetOf(EntityHandle entityHandle);
        void View(ViewCallback callback) override;
        void ApplyCleanup() override;
        void Clear() override;

        void DispatchAwake() override;
        void DispatchStart() override;
        void DispatchUpdate(const float& deltaTime) override;
        void DispatchRender(sf::RenderTarget &renderTarget) override;
        void DispatchDestroy() override;
    };

} // Engine

#include "ComponentSystem.tpp"
