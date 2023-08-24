//
// Created by Flo on 25/11/2022.
//

#include "Scene.hpp"
#include "../../Core/Physics/Physics.hpp"

namespace KrabEngine
{
    Entity Scene::CreateEntity()
    {
        Entity entity = { m_registry.CreateEntity(), this };
        return entity;
    }

    void Scene::DestroyEntity(const Entity& entity) { DestroyEntity(entity.GetHandle()); }
    void Scene::DestroyEntity(const EntityHandle& entityHandle)
    {
        m_registry.DestroyEntity(entityHandle);
    }

    void Scene::OnStart() {

    }

    void Scene::OnUpdate(Timestep DeltaTime)
    {
        m_registry.UpdateAllUpdatable(DeltaTime);
        for (const auto& [Type, Simulator] : m_PhysicsSimulators) {
            Simulator->Simulate(DeltaTime, &m_registry);
        }
    }

    void Scene::OnStop()
    {

    }

    void Scene::RenderScene(sf::RenderTarget &renderTarget)
    {
        auto RenderableSystems = m_registry.GetAllRenderableSystems();
        if (nullptr == m_ActiveCamera)
        {
            for (auto& [Class, RenderableSystem] : RenderableSystems)
            {
                RenderableSystem->DispatchRender(
                    renderTarget
                );
            }
            return;
        }
        m_ActiveCamera->Render(renderTarget, RenderableSystems);
    }

    // NOTE : le problème principal de cette implémentation de destruction est global à toutes les méthodes de destruction des composants
    // qui se trouve dans l'Engine : les pointeurs vers les composants récupérés via GetComponentOf deviennes invalides
    // soucis qui pourrait être résolu en utilisant un smart pointer semblable à weak_ptr mais pour des std::unique_ptr
    // il faut également revoir le système d'attribution de Handles afin de libérer les handles lors de la destruction d'une entité ou d'un composant
    void Scene::Clear()
    {
        m_registry.DestroyAll();

        delete m_ActiveCamera;
        m_ActiveCamera = nullptr;
        // @todo replace with unique ptrs?
        for (auto [_, Simulator] : m_PhysicsSimulators) {
            delete Simulator;
        }
        m_PhysicsSimulators.clear();
    }

    Scene::~Scene() {
        Clear();
    }

    void Scene::AddPhysicsSimulator(KrabEngine::Physics::IPhysicsSimulator *Simulator) {
        auto SimulatorType = Simulator->GetBodyType();
        if (m_PhysicsSimulators.contains(SimulatorType)) {
            throw std::runtime_error("Target physics component types already has a simulator.");
        }

        m_PhysicsSimulators.insert({SimulatorType, Simulator});
    }

    void Scene::RemovePhysicsSimulator(KrabEngine::Physics::IPhysicsSimulator *Simulator) {
        auto SimulatorType = Simulator->GetBodyType();
        if (m_PhysicsSimulators.contains(SimulatorType)) {
            m_PhysicsSimulators.erase(SimulatorType);
        }
    }

    void Scene::ApplyCleanup()
    {
        m_registry.ApplyCleanup();
    }
} // Engine