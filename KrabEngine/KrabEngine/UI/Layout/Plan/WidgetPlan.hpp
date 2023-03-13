//
// Created by Admin on 01/03/2023.
//

#pragma once


#include "../../Widget/Widget.hpp"
#include "../../../Core/Application/EngineApplication.hpp"

namespace KrabEngine::UI
{
    class WidgetPlan : public Widget {
    public:
        DECLARE_CLASS_TYPE(WidgetPlan, Widget)

        void Init(const sf::Vector2f& position = {0, 0}, const sf::Vector2f& size = {
                static_cast<float>(Engine::EngineApplication::Get()->GetWindow().getSize().x),
                static_cast<float>(Engine::EngineApplication::Get()->GetWindow().getSize().y)
        }
        )
        {
            SetPosition(position);
            SetSize(size);
        }

        void AddChild(Widget *child, int index) override
        {
            Widget::AddChild(child, index);
        }

        void SetPosition(const sf::Vector2f& position) override
        {
            m_previousPosition = m_position;
            Widget::SetPosition(position);
        }

        void SetSize(const sf::Vector2f& size) override
        {
            m_previousSize = m_size;
            Widget::SetSize(size);
        }
    private:
        sf::Vector2f m_previousPosition {0, 0};
        sf::Vector2f m_previousSize {0, 0};

        void UpdatePosition() override
        {
            auto delta = GetPosition() - m_previousPosition;

            auto children = GetChildren();
            for (auto& child : children)
            {
                child->SetPosition(child->GetPosition() + delta);
            }
        }

        void UpdateSize() override
        {
            if (m_previousSize.x == 0 || m_previousSize.y == 0) return;

            auto RatioX = GetSize().x / m_previousSize.x;
            auto RatioY = GetSize().y / m_previousSize.y;
            if (RatioX == 1 && RatioY == 1) return;

            auto children = GetChildren();
            for (auto& child : children)
            {
                // @todo : this does not seem to work for now with nested Vertical/HorizontalBoxes...
//                child->SetSize({
//                   child->GetSize().x * RatioX,
//                   child->GetSize().y * RatioY,
//                });
//                child->SetPosition({
//                   GetPosition().x + (child->GetPosition().x - GetPosition().x) * RatioX,
//                   GetPosition().y + (child->GetPosition().y - GetPosition().y) * RatioY,
//                });
            }
        }
    };
}
