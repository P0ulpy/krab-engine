//
// Created by Flo on 27/02/2023.
//

#pragma once

#include <string_view>
#include <memory>

#include <SFML/Graphics/RenderTarget.hpp>

#include "../../ECS/Scene/Scene.hpp"
#include "SceneInitializer/SceneInitializer.hpp"

namespace KrabEngine
{
    class ScenesLayer;

    struct WaitingForSceneLoad
    {
        bool waiting = false;
        std::string_view name {};
    };

    /**
     * @brief The ScenesSystem class is a singleton that manages the scenes.
     * It is responsible for loading and unloading scenes.
     */
    class ScenesSystem
    {
    public:
        [[nodiscard]] static ScenesSystem* Get();

    private:
        static ScenesSystem* s_Instance;

    public:
        ScenesSystem() = delete;
        explicit ScenesSystem(ScenesLayer* scenesLayer);
        ~ScenesSystem();

        template <class TSceneInitializer>
        void DeclareSceneInitializer(const std::string_view& name);

        void LoadScene(const std::string_view& name);

        [[nodiscard]] bool IsWaitingForSceneLoad() const { return m_waitingForSceneLoad.waiting; }

        [[nodiscard]] ScenesLayer* GetScenesLayer() const { return m_scenesLayer; }
        [[nodiscard]] Scene* GetActiveScene() const { return m_activeScene.get(); }

    private:
        void ApplyLoadScene();
        void UnloadActiveScene();

    private:
        WaitingForSceneLoad m_waitingForSceneLoad {
            .waiting = false,
            .name = {}
        };

        std::unordered_map<std::string_view, SceneInitializer*> m_scenesInitializers {};
        std::unique_ptr<Scene> m_activeScene { nullptr };
        std::string_view m_activeSceneName {};

        ScenesLayer* m_scenesLayer = nullptr;

        friend class EngineApplication;
    };
} // Engine

#include "ScenesSystem.tpp"