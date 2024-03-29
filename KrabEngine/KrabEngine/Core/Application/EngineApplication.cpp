#include "EngineApplication.hpp"
#include "../Inputs/WindowEvents.hpp"
#include "../ScenesSystem/ScenesLayer/ScenesLayer.hpp"

KrabEngine::EngineApplication* KrabEngine::EngineApplication::s_Instance = nullptr;

KrabEngine::EngineApplication* KrabEngine::EngineApplication::Get()
{
    return s_Instance;
}

KrabEngine::EngineApplication::EngineApplication() {
    if(s_Instance)
        throw std::runtime_error("An instance of EngineApplication already exists");
    else
        s_Instance = this;

    m_window.create(sf::VideoMode(800, 800), "Engine Window", sf::Style::Close | sf::Style::Resize);

    m_scenesLayer = std::make_unique<ScenesLayer>(&m_window, "ScenesLayer");
    PushLayer(m_scenesLayer.get());
}

KrabEngine::EngineApplication::~EngineApplication()
{
    KrabEngine::EngineApplication::s_Instance = nullptr;
}

void KrabEngine::EngineApplication::PushLayer(KrabEngine::ApplicationLayer *layer)
{
    if(m_running)
    {
        layer->OnAttach();
        Logger::Log("Layer `", layer->GetName(), "` attached");
    }

    m_layers.push_back(layer);
}

void KrabEngine::EngineApplication::RemoveLayer(KrabEngine::ApplicationLayer *layer)
{
    layer->OnDetach();
    Logger::Log("Layer `", layer->GetName(), "` detached");
    m_layers.erase(std::find(m_layers.begin(), m_layers.end(),layer));
}

void KrabEngine::EngineApplication::Init()
{
    Logger::SetThreadLabel("main");

    for(auto* layer : m_layers)
    {
        layer->OnAttach();
        Logger::Log("Layer `", layer->GetName(), "` attached");
    }

    m_initialized = true;
}

void KrabEngine::EngineApplication::Run()
{
    if(!m_initialized)
    {
        Logger::Err("EngineApplication::Run() : EngineApplication is not initialized ! Please call EngineApplication::Init() before");
        return;
    }

    m_running = true;

    while (m_window.isOpen())
    {
        if(!m_running)
            continue;

        // Avoid loop of death
        Timestep timeStep = std::min(Time::Get()->RestartDeltaTimeClock(), FixedDeltaTime * 2);

        WindowEvents::ProcessEvents(m_window);

        m_window.clear(sf::Color::Black);

        for(ApplicationLayer* layer : m_layers)
        {
            layer->OnUpdate(timeStep);
        }

        // TODO : Support ImGui
#ifdef IMGUI_SUPPORT
        m_imGuiLayer.Begin();
        for(ApplicationLayer* layer : m_layers)
        {
            layer->OnImGuiRender();
        }
        m_imGuiLayer.End();
#endif
        m_window.display();

        // Components & Entities cleanup
        auto* activeScene = ScenesSystem::Get()->GetActiveScene();
        if(activeScene)
            activeScene->ApplyCleanup();

        if(ScenesSystem::Get()->IsWaitingForSceneLoad())
            ScenesSystem::Get()->ApplyLoadScene();

        if(WindowEvents::GetEvent(sf::Event::Closed))
            Stop();

        const auto Elapsed = Time::Get()->GetDeltaTime();
        if (Elapsed < FixedDeltaTime)
        {
            sf::sleep(sf::milliseconds((int) (FixedDeltaTime - Elapsed)));
        } else {
            //KrabEngine::Logger::Warn("Exceeding fixed update time :", Elapsed);
        }
    }
}

void KrabEngine::EngineApplication::Stop()
{
    m_running = false;
    m_window.close();
}
