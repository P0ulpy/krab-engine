//
// Created by Flo on 01/12/2022.
//

#include "ScenesLayer.hpp"
#include "../../Application/EngineApplication.hpp"

#include <memory>
#include <utility>

namespace KrabEngine
{
    ScenesLayer::ScenesLayer(sf::RenderTarget* renderTarget, const std::string_view &name)
        : ApplicationLayer::ApplicationLayer(name)
        , m_renderTarget(renderTarget)
    {

    }

    void ScenesLayer::OnAttach()
    {

    }

    void ScenesLayer::OnDetach()
    {

    }

    void ScenesLayer::OnUpdate(Timestep ts)
    {
        if(!m_scenesSystem.GetActiveScene())
            return;

        m_scenesSystem.GetActiveScene()->OnUpdate(ts);
        m_scenesSystem.GetActiveScene()->RenderScene(*m_renderTarget);
    }

} // Engine