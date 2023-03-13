//
// Created by Flo on 22/02/2023.
//

#include "WidgetVerticalBox.hpp"

namespace KrabEngine::UI {

    void WidgetVerticalBox::Init(const sf::Vector2f &position, const float spacing) {
        m_spacing = spacing;
        SetPosition(position);
    }

    void WidgetVerticalBox::UpdatePosition() {
        auto position = GetPosition();
        float y = position.y;
        auto children = GetChildren();

        for (auto& child : children) {
            child->SetPosition({position.x, y});
            y += child->GetSize().y + m_spacing;
        }

        RecomputeSize();
    }

    void WidgetVerticalBox::RecomputeSize() const {
        auto children = GetChildren();
        float width = 0.0f;
        float height = 0.0f;

        for (auto& child : children) {
            width = std::max(width, child->GetSize().x);
            height += child->GetSize().y + m_spacing;
        }

        height -= m_spacing;
        height = std::max(height, 0.f);

        m_size = {width, height};
    }

    void WidgetVerticalBox::UpdateSize()
    {
        if (m_previousSize.y == 0) return;
        auto RatioX = GetSize().x / m_previousSize.x;
        m_spacing = m_spacing * RatioX;

        for (auto& child : GetChildren()) {
            auto ChildSize = child->GetSize();
            auto NewChildX = (ChildSize.x / m_previousSize.x) * GetSize().x;
            child->SetSize({
               NewChildX,
               (ChildSize.y / ChildSize.x) * NewChildX
            });
        }

        UpdatePosition();
    }
} // UI