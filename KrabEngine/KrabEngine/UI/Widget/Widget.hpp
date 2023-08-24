//
// Created by Flo on 21/02/2023.
//

#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "../../Core/Components/CompositeComponent.hpp"

namespace KrabEngine::UI
{
    class Widget : public CompositeComponent<Widget>
    {
    public:
        DECLARE_CLASS_TYPE(Widget, CompositeComponent<Widget>)

        virtual void OnRender(sf::RenderTarget& renderTarget) {
            if(!IsActive())
                return;

            for (auto& child : GetChildren())
                child->OnRender(renderTarget);
        }

        virtual void AddChild(Widget* child, int index);
        virtual void RemoveChild(Widget* child) override;

        virtual void SetPosition(const sf::Vector2f& position);
        virtual void SetSize(const sf::Vector2f& size);

        [[nodiscard]] const sf::Vector2f& GetPosition() const   { return m_position; }
        [[nodiscard]] const sf::Vector2f& GetSize() const       { return m_size; }

        virtual void UpdatePosition()   {}
        virtual void UpdateSize()       {}
        virtual void RecomputeSize() const       {}

        void SetLayoutIndex(int index) { m_layoutIndex = index; }
        [[nodiscard]] int GetLayoutIndex() const { return m_layoutIndex; }

    protected:
        mutable sf::Vector2f m_position { .0f, .0f };
        mutable sf::Vector2f m_size { .0f, .0f };
        mutable int m_layoutIndex{ 0 };
    };
} // KrabEngine::UI
