//
// Created by Flo on 18/01/2023.
//

#include "../../Core/Logger/Logger.hpp"
#include "../../UI/Canvas/WidgetCanvas.hpp"

namespace KrabEngine
{
    template<class TComponent>
    Component *ComponentSystem<TComponent>::Add(
        EntityHandle entityHandle,
        Scene* scene
    )
    {
        if(components.contains(entityHandle))
        {
            Logger::Err("this entity ", entityHandle ," already has a (", TComponent::getClassType()->getTypeName(), ") allocated for him");
            return nullptr;
        }

        components.insert_or_assign(entityHandle, TComponent());
        components[entityHandle].m_handle = Core::UUID::CreateNew();
        components[entityHandle].m_entityHandle = entityHandle;
        components[entityHandle].m_scene = scene;

        if constexpr (requires (TComponent& c) { c.OnAwake(); })
            components[entityHandle].OnAwake();

        return static_cast<Component*>(&components[entityHandle]);
    }

    template<class TComponent>
    void ComponentSystem<TComponent>::RemoveAfter(EntityHandle entityHandle)
    {
        /*if(std::find(componentsToBeCleaned.begin(), componentsToBeCleaned.end(), entityHandle) != componentsToBeCleaned.end())
            return;*/

        componentsToBeCleaned.push_back(entityHandle);
    }

    template<class TComponent>
    void ComponentSystem<TComponent>::Remove(EntityHandle entityHandle)
    {
        typename std::unordered_map<EntityHandle, TComponent>::iterator itr;
        Remove(entityHandle, itr);
    }

    template<class TComponent>
    void ComponentSystem<TComponent>::Remove(EntityHandle entityHandle, typename std::unordered_map<EntityHandle, TComponent>::iterator& removedComponentIt)
    {
        if constexpr (requires (TComponent& c) { c.OnDestroy(); })
            components[entityHandle].OnDestroy();

        // Remove from this component from his parent children if TComponent extends from CompositeComponent
        // NOTE : (maybe use std::is_base_of_v instead of requires)
        if constexpr (requires (TComponent& c) { c.GetParent(); c.GetChildren(); })
        {
            for(auto& child : components[entityHandle].GetChildren())
                child->SetParent(nullptr);

            auto* parent = components[entityHandle].GetParent();
            if(parent)
            {
                parent->RemoveChild(&components[entityHandle]);
            }
        }

        auto toRemove = components.find(entityHandle);
        if(toRemove != components.end())
            removedComponentIt = components.erase(toRemove);
    }

    template<class TComponent>
    bool ComponentSystem<TComponent>::Has(EntityHandle entityHandle)
    {
        return components.contains(entityHandle);
    }

    template<class TComponent>
    Component* ComponentSystem<TComponent>::Get(EntityHandle entityHandle)
    {
        return static_cast<Component*>(GetOf(entityHandle));
    }

    template<class TComponent>
    TComponent* ComponentSystem<TComponent>::GetOf(EntityHandle entityHandle)
    {
        return &components[entityHandle];
    }

    template<class TComponent>
    void ComponentSystem<TComponent>::View(IComponentSystem::ViewCallback callback)
    {
        for(auto& [ handle, component ] : components)
        {
            callback(&component);
        }
    }

    template<class TComponent>
    void ComponentSystem<TComponent>::ApplyCleanup()
    {
        for(auto& handle : componentsToBeCleaned)
        {
            Remove(handle);
        }

        componentsToBeCleaned.clear();
    }

    template<class TComponent>
    void ComponentSystem<TComponent>::Clear()
    {
        for (auto it = components.begin(); it != components.end(); )
        {
            Remove(it->first, it);
        }
    }

    template<class TComponent>
    ComponentSystem<TComponent>::~ComponentSystem()
    {
        Clear();
    }

    // Dispatch

    template<class TComponent>
    void ComponentSystem<TComponent>::DispatchAwake()
    {
        if constexpr (requires (TComponent& c) { c.OnAwake(); })
        {
            for(auto& [ handle, component ] : components)
            {
                component.OnAwake();
            }
        }
    }

    template<class TComponent>
    void ComponentSystem<TComponent>::DispatchStart()
    {
        if constexpr (requires (TComponent& c) { c.OnStart(); })
        {
            for(auto& [ handle, component ] : components)
            {
                component.OnStart();
            }
        }
    }

    template<class TComponent>
    void ComponentSystem<TComponent>::DispatchUpdate(const float &deltaTime)
    {
        if constexpr (requires (TComponent& c) { c.OnUpdate(0.0f); })
        {
            for(auto& [ handle, component ] : components)
            {
                if(component.m_active)
                    component.OnUpdate(deltaTime);
            }
        }
    }

    template<class TComponent>
    void ComponentSystem<TComponent>::DispatchRender(sf::RenderTarget &renderTarget)
    {
        if constexpr (requires (TComponent& c, sf::RenderTarget& rT) { c.OnRender(rT); })
        {
            for(auto& [ handle, component ] : components)
            {
                if(!component.m_active)
                    continue;

                if constexpr (std::is_base_of_v<UI::Widget, TComponent>)
                {
                    if constexpr (std::is_base_of_v<UI::WidgetCanvas, TComponent>)
                    {
                        auto ActiveScene = component.GetEntity().GetScene();
                        if (nullptr != ActiveScene->GetActiveCamera())
                        {
                            ActiveScene->GetActiveCamera()->SetUICanvasSize(&component);
                        }
                        component.OnRender(renderTarget);
                    }
                }
                else
                    component.OnRender(renderTarget);
            }
        }
    }

    template<class TComponent>
    void ComponentSystem<TComponent>::DispatchDestroy()
    {
        if constexpr (requires (TComponent& c) { c.OnDestroy(); })
        {
            for(auto& [ handle, component ] : components)
            {
                component.OnDestroy();
            }
        }
    }
} // Engine