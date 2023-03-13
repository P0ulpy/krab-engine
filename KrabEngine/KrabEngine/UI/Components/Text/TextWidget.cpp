//
// Created by Flo on 22/02/2023.
//

#include "TextWidget.hpp"

namespace KrabEngine::UI {

        void TextWidget::OnUpdate(float deltaTime) {}

        void TextWidget::OnRender(sf::RenderTarget &renderTarget) {
            renderTarget.draw(m_text);
        }

        void TextWidget::Init(const std::string &text, sf::Font *font, const sf::Vector2f &position,
                              unsigned int characterSize, const sf::Color &color) {
            m_text.setFont(*font);
            m_text.setCharacterSize(characterSize);
            m_text.setFillColor(color);
            m_text.setString(text);

            SetPosition(position);
            SetSize(sf::Vector2f{m_text.getGlobalBounds().width, m_text.getGlobalBounds().height});
        }

        void TextWidget::SetPosition(const sf::Vector2f &position) {
            Widget::SetPosition(position);
            m_text.setPosition(position);
        }

    void TextWidget::SetText(const std::string_view &newText)
    {
        m_text.setString(newText.data());
        SetSize(sf::Vector2f{m_text.getGlobalBounds().width, m_text.getGlobalBounds().height});
    }
} // UI