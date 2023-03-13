//
// Created by Flo on 15/12/2022.
//

#pragma once

#include <functional>

#include <SFML/Graphics/RenderTarget.hpp>

#include "../Component/Component.hpp"

namespace KrabEngine
{
    class Scene;
    class IComponentSystem
    {
    public:
        virtual ~IComponentSystem() = default;

        virtual Component* Add(EntityHandle entityHandle, Scene* scene) = 0;
        virtual void RemoveAfter(EntityHandle entityHandle) = 0;
        virtual void Remove(EntityHandle entityHandle) = 0;
        virtual bool Has(EntityHandle entityHandle) = 0;
        virtual Component* Get(EntityHandle entityHandle) = 0;
        virtual bool NeedCleanup() = 0;
        virtual void ApplyCleanup() = 0;
        virtual void Clear() = 0;

        virtual void DispatchAwake() = 0;
        virtual void DispatchStart() = 0;
        virtual void DispatchUpdate(const float& deltaTime) = 0;
        virtual void DispatchRender(sf::RenderTarget &renderTarget) = 0;
        virtual void DispatchDestroy() = 0;

        using ViewCallback = std::function<void(Component*)>;

        virtual void View(ViewCallback callback) = 0;
    };
}
