//
// Created by Flo on 22/02/2023.
//

#pragma once

#include <functional>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Mouse.hpp>

#include "../../../Core/Inputs/Connection.hpp"
#include "../../../Core/Application/EngineApplication.hpp"
#include "../../Widget/Widget.hpp"
#include "../../../Core/Inputs/InputSignal.hpp"

namespace KrabEngine::UI
{
    class ButtonWidget : public Widget
    {
    public:
        DECLARE_CLASS_TYPE(ButtonWidget, Widget)

        using Callback = std::function<void()>;

        explicit ButtonWidget();
        virtual void OnUpdate(const float& deltaTime);
        virtual void OnRender(sf::RenderTarget& renderTarget);
        void Init(const sf::Vector2f& position, const sf::Vector2f& size);
        void SetPosition(const sf::Vector2f& position) override;
        void SetSize(const sf::Vector2f& size) override;

        virtual void SetButtonColor(const sf::Color& color);
        void SetDefaultColor(const sf::Color& color)    { m_defaultColor = color; }
        void SetHoverColor(const sf::Color& color)      { m_hoverColor = color; }
        void SetClickedColor(const sf::Color& color)    { m_clickedColor = color; }

        void SetOnClick(const Callback& onClick);
        virtual void OnClick();

    protected:
        sf::RectangleShape m_button { };

        sf::Color m_defaultColor = sf::Color::White;
        sf::Color m_hoverColor = sf::Color::Red;
        sf::Color m_clickedColor = sf::Color::Green;

        bool m_isHovered = false;

        Callback m_onClick;
        SignalSystem::ScopedConnectionSignal m_OnClickConnection;
    };
}
