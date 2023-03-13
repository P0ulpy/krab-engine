//
// Created by Flo on 21/02/2023.
//

#include "Widget.hpp"

namespace KrabEngine::UI
{
    void Widget::AddChild(Widget *child, int index)
    {
        child->SetLayoutIndex(index);
        CompositeComponent<Widget>::AddChild(child);
        UpdatePosition();
    }

    void Widget::RemoveChild(Widget *child)
    {
        CompositeComponent<Widget>::RemoveChild(child);
        UpdatePosition();
    }

    void Widget::SetPosition(const sf::Vector2f& position)
    {
        m_position = position;
        UpdatePosition();
    }

    void Widget::SetSize(const sf::Vector2f &size)
    {
        m_size = size;
        UpdateSize();
        RecomputeSize();
    }
} // Engine::UI