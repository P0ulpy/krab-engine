//
// Created by Flo on 22/02/2023.
//

#pragma once

#include "../../Widget/Widget.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Text.hpp"

namespace KrabEngine::UI {

        class TextWidget : public Widget {
        public:
            DECLARE_CLASS_TYPE(TextWidget, Widget)

            virtual void OnUpdate(float deltaTime);
            void OnRender(sf::RenderTarget& renderTarget) override;

            void Init(const std::string& text, sf::Font* font, const sf::Vector2f& position, unsigned int characterSize = 30,  const sf::Color& color = sf::Color::White);

            void SetText(const std::string_view& newText);
            void SetPosition(const sf::Vector2f& position) override;

        private:
            sf::Text m_text;
        };

} // UI
