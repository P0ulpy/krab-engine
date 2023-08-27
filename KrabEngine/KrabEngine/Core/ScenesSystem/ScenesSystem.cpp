//
// Created by Flo on 27/02/2023.
//

#include "ScenesSystem.hpp"

namespace KrabEngine
{
    ScenesSystem* ScenesSystem::s_Instance = nullptr;

    ScenesSystem *ScenesSystem::Get()
    {
        if(!s_Instance)
        {
            Logger::Err("ScenesSystem::Get() : ScenesSystemLayer is not initialized !");
            return nullptr;
        }
        else
            return s_Instance;
    }

    ScenesSystem::ScenesSystem(ScenesLayer* scenesLayer)
        : m_scenesLayer(scenesLayer)
    {
        if(s_Instance)
            Logger::Err("ScenesSystem::ScenesSystem() : ScenesSystemLayer is already initialized !");
        else
            s_Instance = this;
    }

    ScenesSystem::~ScenesSystem()
    {
        UnloadActiveScene();
        for(auto& [name, initializer] : m_scenesInitializers)
            delete initializer;

        m_scenesInitializers.clear();
        s_Instance = nullptr;
    }

    void ScenesSystem::LoadScene(const std::string_view &name)
    {
        if(!m_scenesInitializers.contains(name))
        {
            Logger::Err("ScenesSystemLayer::LoadScene() : SceneInitializer ", name, " is not declared !");
            return;
        }

        m_waitingForSceneLoad = {
            .waiting = true,
            .name = name,
        };
    }

    void ScenesSystem::ApplyLoadScene()
    {
        if(m_activeScene)
            UnloadActiveScene();

        m_activeScene = std::make_unique<Scene>();
        m_activeSceneName = m_waitingForSceneLoad.name;
        m_scenesInitializers[m_activeSceneName]->OnLoaded(m_activeScene.get());

        m_waitingForSceneLoad = {
            .waiting = false,
            .name = {},
        };

        Logger::Log("Scene `", m_activeSceneName, "` loaded");
    }

    void ScenesSystem::UnloadActiveScene()
    {
        m_scenesInitializers[m_activeSceneName]->OnUnloaded(m_activeScene.get());
        m_activeScene->Clear();
        m_activeScene = nullptr;

        Logger::Log("Scene `", m_activeSceneName, "` unloaded");

        m_activeSceneName = {};
    }

} // KrabEngine