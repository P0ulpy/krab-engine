//
// Created by Flo on 22/02/2023.
//

#include "SpriteWidget.hpp"

namespace KrabEngine::UI {
    void SpriteWidget::OnRender(sf::RenderTarget &renderTarget) {
        renderTarget.draw(m_sprite);
    }

    void SpriteWidget::Init(const sf::Texture &texture, const sf::Vector2f &position) {
        m_sprite.setTexture(texture);
        SetPosition(position);
        RecomputeSize();
    }

    void SpriteWidget::SetPosition(const sf::Vector2f &position) {
        Widget::SetPosition(position);
        m_sprite.setPosition(position);
    }

    void SpriteWidget::RecomputeSize() const
    {
        m_size = { m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height };
    }

    void SpriteWidget::SetScale(const sf::Vector2f &scale)
    {
        m_sprite.setScale(scale);
        RecomputeSize();
    }

    void SpriteWidget::SetSize(const sf::Vector2f &size) {
        Widget::SetSize(size);
        m_sprite.setScale(size.x / m_sprite.getTextureRect().width, size.y / m_sprite.getTextureRect().height);
    }
} // UI