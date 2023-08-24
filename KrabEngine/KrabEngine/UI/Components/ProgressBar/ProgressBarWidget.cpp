//
// Created by Admin on 28/02/2023.
//

#include "ProgressBarWidget.hpp"

namespace KrabEngine::UI {

    void ProgressBarWidget::OnUpdate(KrabEngine::Timestep ts) {
        m_FillSprite.setScale((GetSize().x - m_border.x *2) * (m_Value / m_MaxValue) / m_FillGlobalBounds.x, (GetSize().y - m_border.y*2) / m_FillGlobalBounds.y);
    }

    void ProgressBarWidget::OnRender(sf::RenderTarget &renderTarget) {
        renderTarget.draw(m_FillSprite);
        renderTarget.draw(m_BackgroundSprite);
    }

    void ProgressBarWidget::Init(const sf::Vector2f &position, const sf::Vector2f &size
                                 , const sf::Texture &backgroundTexture, const sf::Texture &fillTexture
                                 , sf::Vector2f border) {
        m_BackgroundSprite.setTexture(backgroundTexture);
        m_FillSprite.setTexture(fillTexture);

        m_border = border;

        SetPosition(position);
        SetSize(size);
    }

    void ProgressBarWidget::SetPosition(const sf::Vector2f &position) {
        Widget::SetPosition(position);
        m_BackgroundSprite.setPosition(position);
        m_FillSprite.setPosition(position + m_border);
    }

    void ProgressBarWidget::SetSize(const sf::Vector2f &size) {
        Widget::SetSize(size);
        m_BackgroundSprite.setScale(size.x / m_BackgroundSprite.getGlobalBounds().width, size.y / m_BackgroundSprite.getGlobalBounds().height);

        m_FillGlobalBounds = sf::Vector2f(m_FillSprite.getGlobalBounds().width, m_FillSprite.getGlobalBounds().height);
    }
}
