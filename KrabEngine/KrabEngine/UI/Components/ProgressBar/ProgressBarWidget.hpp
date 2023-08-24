//
// Created by Admin on 28/02/2023.
//

#pragma once

#include "../../Widget/Widget.hpp"
#include "../../../Core/Time.hpp"

#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

namespace KrabEngine::UI
{
    class ProgressBarWidget : public Widget {
    public:
        DECLARE_CLASS_TYPE(ProgressBarWidget, Widget)

        explicit ProgressBarWidget() = default;

        virtual void OnUpdate(KrabEngine::Timestep ts);
        void OnRender(sf::RenderTarget& renderTarget);

        void Init(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Texture& backgroundTexture, const sf::Texture& fillTexture, sf::Vector2f border = {0.5f, 0.5f});

        void SetValue(float value) { m_Value = value; }
        float GetValue() const { return m_Value; }

        void SetMaxValue(float value) { m_MaxValue = value; }
        float GetMaxValue() const { return m_MaxValue; }

        void SetPosition(const sf::Vector2f& position) override;
        void SetSize(const sf::Vector2f& size) override;

    protected:
        // TODO : should be replaced with a float between 0 and 1
        float m_Value = 0.0f;
        float m_MaxValue = 100.0f;

        sf::Vector2f m_border = {0.5f, 0.5f};

        sf::Sprite m_BackgroundSprite { };
        sf::Sprite m_FillSprite { };

        sf::Vector2f m_FillGlobalBounds { };
    };
}
