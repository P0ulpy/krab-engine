#pragma once

namespace KrabEngine
{
    template<class TSceneInitializer>
    void ScenesSystem::DeclareSceneInitializer(const std::string_view &name)
    {
        static_assert(std::is_base_of_v<SceneInitializer, TSceneInitializer>, "TSceneInitializer must inherit from SceneInitializer");

        if(m_scenesInitializers.contains(name))
        {
            Logger::Warn("ScenesSystemLayer::DeclareSceneInitializer() : SceneInitializer `", name, "` is already declared ! You are about to override it !");
            delete m_scenesInitializers[name];
        }

        m_scenesInitializers[name] = new TSceneInitializer();

        Logger::Log("SceneInitializer `", name, "` is declared");
    }
}