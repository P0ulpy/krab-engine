//
// Created by Flo on 22/02/2023.
//

#pragma once

#include "../ButtonWidget.hpp"
#include "SFML/Graphics/Sprite.hpp"

namespace KrabEngine::UI {

        class SpriteButtonWidget : public ButtonWidget {
        public:
            DECLARE_CLASS_TYPE(SpriteButtonWidget, ButtonWidget)

            void OnRender(sf::RenderTarget& renderTarget) override;

            void Init(const sf::Texture* texture, const sf::Vector2f& position, const float& border = 0.0f);

            void SetButtonColor(const sf::Color& color) override {}

            void SetPosition(const sf::Vector2f& position) override;
            void SetSize(const sf::Vector2f& size) override      { ButtonWidget::SetSize(size); }

        private:
            sf::Sprite m_sprite { };
            float m_border = 0.0f;
        };
} // UI
