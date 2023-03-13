//
// Created by Flo on 26/11/2022.
//

#include "EntitiesRegistry.hpp"
#include "../../Core/Components/Physics.hpp"
#include "../../Maths/Collisions.hpp"

namespace KrabEngine
{
    EntityHandle EntitiesRegistry::CreateEntity()
    {
        return EntityHandle(Core::UUID::CreateNew());
    }

    void EntitiesRegistry::DestroyEntity(EntityHandle handle)
    {
        for(auto& [classType, system] : m_componentSystems)
        {
            if(system->Has(handle))
            {
                system->RemoveAfter(handle);
            }
        }
    }

    void EntitiesRegistry::AwakeAll()
    {
        for(auto& [classType, system] : m_componentSystems)
        {
            system->DispatchAwake();
        }
    }

    void EntitiesRegistry::StartAll()
    {
        for(auto& [classType, system] : m_componentSystems)
        {
            system->DispatchStart();
        }
    }

    void EntitiesRegistry::UpdateAllUpdatable(const float& deltaTime)
    {
        for(auto& [classType, system] : m_updatableSystems)
        {
            system->DispatchUpdate(deltaTime);
        }
    }

    std::unordered_map<RTTI::ClassType*, IComponentSystem*> EntitiesRegistry::GetAllRenderableSystems()
    {
        return m_renderableSystems;
    }

    void EntitiesRegistry::ApplyCleanup()
    {
        for(auto& [classType, system] : m_componentSystems)
        {
            if(system->NeedCleanup())
                system->ApplyCleanup();
        }
    }

    void EntitiesRegistry::DestroyAll()
    {
        m_updatableSystems.clear();
        m_renderableSystems.clear();
        m_componentSystems.clear();
    }
} // Engine