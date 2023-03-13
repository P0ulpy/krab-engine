//
// Created by Flo on 25/11/2022.
//

#pragma once

#include "../../Core/UUID.hpp"
#include "../Handles/Handles.hpp"
#include "../Entity/Entity.hpp"
#include "../../RTTI/ClassType.hpp"

namespace KrabEngine
{
    template <typename ...ComponentsT>
    struct ComponentRequirements {};

    template<typename RequirementsList>
    struct ComponentRequirementsTraversor;

    template<template <class ...> typename TList, class ...T>
    struct ComponentRequirementsTraversor<TList<T...>>
    {
        template <typename Fn>
        void operator()(Fn Traversor)
        {
            Traversor.template operator()<T...>();
        }
    };
    
    /**
     * @brief Base class for all components.
     * A component is a class that can be attached to an entity.
     * It is used to add functionality to an entity.
     * @note A component can not exist without an entity.
     * @note A component inherited class need to use the macro DECLARE_CLASS_TYPE(MyComponent, Ancestor) to be able to be instantiated correctly.
     */
    class Component : public RTTI::IClassType
    {
    public:
        DECLARE_CLASS_TYPE(Component, RTTI::NoClassTypeAncestor)

        using RequiredComponents = ComponentRequirements<>;

    public:
        Component() = default;

        /* Implementable Component Events
        void OnAwake();
        void OnStart();
        void OnUpdate(const float& deltaTime);
        void OnRender(sf::RenderTarget &renderTarget);
        void OnImGuiRender();
        void OnDestroy();
        */

        virtual void SetActive(bool active)     { m_active = active; }
        [[nodiscard]] bool IsActive() const     { return m_active; }
        [[nodiscard]] Entity& GetEntity() { m_entity = Entity(m_entityHandle, m_scene); return m_entity; }
        [[nodiscard]] const Entity& GetEntity() const { m_entity = Entity(m_entityHandle, m_scene); return m_entity; }
        [[nodiscard]] Scene* GetScene() const { return m_scene; }

        [[nodiscard]] inline EntityHandle GetHandle() const { return m_handle; }

        operator bool() const               { return m_handle != ComponentHandle::Null; }
        operator ComponentHandle() const    { return m_handle; }
        operator uint64_t() const           { return (uint64_t)m_handle; }

        bool operator==(const Component& other) const
        {
            return m_handle == other.m_handle && m_scene == other.m_scene;
        }

        bool operator!=(const Component& other) const
        {
            return !(*this == other);
        }

    private:
        bool m_active = true;
        ComponentHandle m_handle = ComponentHandle::Null;
        EntityHandle m_entityHandle = EntityHandle::Null;
        mutable Entity m_entity { EntityHandle::Null, nullptr };

        // Component can not exist if his scene is destroyed so a raw pointer is fine here
        Scene* m_scene = nullptr;

        template <class TComponent>
        friend class ComponentSystem;
        friend class Scene;
    };

} // Engine
