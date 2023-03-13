//
// Created by Flo on 22/02/2023.
//

#include "WidgetHorizontalBox.hpp"

namespace KrabEngine::UI {

    void WidgetHorizontalBox::Init(const sf::Vector2f &position, const float spacing) {
        m_spacing = spacing;
        SetPosition(position);
    }

    void WidgetHorizontalBox::UpdatePosition() {
        auto position = GetPosition();
        float x = position.x;
        auto children = GetChildren();

        for (auto& child : children) {
            child->SetPosition({x, position.y});
            x += child->GetSize().x + m_spacing;
        }

        RecomputeSize();
    }

    void WidgetHorizontalBox::RecomputeSize() const {
        auto children = GetChildren();
        float width = 0.0f;
        float height = 0.0f;

        for (auto& child : children) {
            width += child->GetSize().x + m_spacing;
            height = std::max(height, child->GetSize().y);
        }

        width -= m_spacing;
        width = std::max(width, 0.f);

        m_size = {width, height};
    }

    void WidgetHorizontalBox::UpdateSize() {
        if (m_previousSize.x == 0) return;

        auto RatioY = GetSize().y / m_previousSize.y;
        m_spacing = m_spacing * RatioY;

        for (auto& child : GetChildren()) {
            auto ChildSize = child->GetSize();
            auto NewChildY = (ChildSize.y / m_previousSize.y) * GetSize().y;
            child->SetSize({
               (ChildSize.x / ChildSize.y) * NewChildY,
               NewChildY
            });
        }

        UpdatePosition();
    }


} // UI