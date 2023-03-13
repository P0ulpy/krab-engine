#pragma once

#include <vector>
#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>

#include "ApplicationLayer.hpp"

namespace KrabEngine
{
    class ScenesLayer;

    class EngineApplication
    {
    public:
        [[nodiscard]] static EngineApplication* Get();

    private:
        static EngineApplication* s_Instance;
        static constexpr float FixedDeltaTime = 16.6666f;

    public:
        EngineApplication();
        ~EngineApplication();

        void Init();
        void Run();
        void Stop();

        void PushLayer(ApplicationLayer* layer);
        void RemoveLayer(ApplicationLayer* layer);

        inline sf::RenderWindow& GetWindow() { return m_window; }
        inline ScenesLayer* GetScenesLayer() { return m_scenesLayer.get(); }

    private:
        sf::RenderWindow m_window;

        bool m_initialized = false;
        bool m_running = false;
        Timestep m_LastFrameTime = 0;

        // TODO : use a custom stack where we can iterate through
        std::vector<ApplicationLayer*> m_layers {};
        std::unique_ptr<ScenesLayer> m_scenesLayer { nullptr };
    };
}