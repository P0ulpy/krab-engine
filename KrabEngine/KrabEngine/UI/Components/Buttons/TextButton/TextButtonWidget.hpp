//
// Created by Flo on 22/02/2023.
//

#pragma once

#include "../ButtonWidget.hpp"
#include "SFML/Graphics/Text.hpp"

namespace KrabEngine::UI
{
    class TextButtonWidget : public ButtonWidget
    {
    public:
        DECLARE_CLASS_TYPE(TextButtonWidget, ButtonWidget)

        void OnRender(sf::RenderTarget& renderTarget) override;

        void Init(const std::string& text, sf::Font* font, const sf::Vector2f& position, const float& border = 0.0f, unsigned int characterSize = 30);

        void SetButtonColor(const sf::Color& color) override;
        void SetText(const std::string& text);

        void SetPosition(const sf::Vector2f& position) override;

    private:
        sf::Text m_text { };
        float m_border = 0;
    };
} // UI
