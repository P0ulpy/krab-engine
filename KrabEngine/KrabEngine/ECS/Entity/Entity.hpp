//
// Created by Flo on 30/01/2023.
//

#pragma once

#include "../Handles/Handles.hpp"

namespace KrabEngine
{
    class Scene;

    class Entity
    {
    public:
        Entity() = default;
        Entity(EntityHandle handle, Scene* scene);

        template<typename T>
        T* AddComponent();

        template<typename T>
        T* GetComponent();

        template<typename T>
        const T* GetComponent() const;

        template<typename T>
        bool HasComponent() const;

        template<typename T>
        void RemoveComponent();

        [[nodiscard]] inline EntityHandle GetHandle() const { return m_handle; }
        [[nodiscard]] Scene* GetScene() const { return m_scene; }

        operator bool() const           { return m_handle != EntityHandle::Null; }
        operator EntityHandle() const   { return m_handle; }
        operator uint64_t() const       { return (uint64_t)m_handle; }

        bool operator==(const Entity& other) const
        {
            return m_handle == other.m_handle && m_scene == other.m_scene;
        }

        bool operator!=(const Entity& other) const
        {
            return !(*this == other);
        }

    private:
        EntityHandle m_handle = EntityHandle::Null;

        // Entity can not exist if his scene is destroyed so a raw pointer is fine here
        Scene* m_scene = nullptr;
    };

} // Engine
