//
// Created by Flo on 22/02/2023.
//

#include "SpriteButtonWidget.hpp"

namespace KrabEngine::UI {
        void SpriteButtonWidget::OnRender(sf::RenderTarget &renderTarget) {
            ButtonWidget::OnRender(renderTarget);
            renderTarget.draw(m_sprite);
        }

        void SpriteButtonWidget::Init(const sf::Texture *texture, const sf::Vector2f &position, const float &border) {
            m_border = border;
            m_sprite.setTexture(*texture);
            SetSize({ m_sprite.getGlobalBounds().width + m_border * 2, m_sprite.getGlobalBounds().height + m_border * 2 });
            SetPosition(position);
        }

        void SpriteButtonWidget::SetPosition(const sf::Vector2f &position) {
            ButtonWidget::SetPosition(position);
            m_sprite.setPosition(position + sf::Vector2f{m_border, m_border});
        }
} // UI