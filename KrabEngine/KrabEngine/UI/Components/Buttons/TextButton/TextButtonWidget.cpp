//
// Created by Flo on 22/02/2023.
//

#include "TextButtonWidget.hpp"

namespace KrabEngine::UI {
        void TextButtonWidget::OnRender(sf::RenderTarget &renderTarget) {
            ButtonWidget::OnRender(renderTarget);
            renderTarget.draw(m_text);
        }

        void TextButtonWidget::Init(const std::string &text, sf::Font *font,
                                    const sf::Vector2f &position,
                                    const float &border, unsigned int characterSize) {
            m_border = border;
            m_text = sf::Text(text, *font, characterSize);
            m_text.setFillColor(m_defaultColor);
            SetText(text);

            sf::Vector2f textButtonSize = sf::Vector2f {
                    m_text.getGlobalBounds().width + m_border * 2,
                    m_text.getGlobalBounds().height + m_border * 2
            };

            SetSize(textButtonSize);
            SetPosition(position);
        }

        void TextButtonWidget::SetPosition(const sf::Vector2f &position) {
            ButtonWidget::SetPosition(position);
            float xPos = (position.x + GetSize().x / 2) - (m_text.getLocalBounds().width / 2);
            float yPos = (position.y + GetSize().y / 2.2f) - (m_text.getLocalBounds().height / 2);
            m_text.setPosition(xPos, yPos);
        }

        void TextButtonWidget::SetButtonColor(const sf::Color &color) {
            m_text.setFillColor(color);
        }

    void TextButtonWidget::SetText(const std::string &text) {
        m_text.setString(text);

        sf::Vector2f textButtonSize = sf::Vector2f {
                m_text.getGlobalBounds().width + m_border * 2,
                m_text.getGlobalBounds().height + m_border * 2
        };

        SetSize(textButtonSize);
    }
} // UI