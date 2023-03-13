//
// Created by Flo on 22/02/2023.
//

#pragma once

#include <map>
#include "../Widget/Widget.hpp"
#include "../../Core/Application/EngineApplication.hpp"

namespace KrabEngine::UI
{
    enum class CanvasAnchors {
        TOP_LEFT = 0,
        TOP_CENTER,
        TOP_RIGHT,
        CENTER_LEFT,
        CENTER,
        CENTER_RIGHT,
        BOTTOM_LEFT,
        BOTTOM_CENTER,
        BOTTOM_RIGHT,
        HORIZONTAL_STRETCH_TOP,
        HORIZONTAL_STRETCH_CENTER,
        HORIZONTAL_STRETCH_BOTTOM,
        VERTICAL_STRETCH_LEFT,
        VERTICAL_STRETCH_CENTER,
        VERTICAL_STRETCH_RIGHT,
        STRETCH
    };

    struct CanvasChildOptions {
        // @todo use also with Anchors::CUSTOM
        static constexpr float ReferenceCanvasHeight = 1080.f;
        static constexpr float ReferenceCanvasWidth = 1920.f;

        CanvasAnchors Anchors = CanvasAnchors::TOP_LEFT;
        float TopOffset = 0.f;
        float BottomOffset = 0.f;
        float LeftOffset = 0.f;
        float RightOffset = 0.f;
        // will have priority if >= 0
        float TopOffsetRatio = -1.f;
        float BottomOffsetRatio = -1.f;
        float LeftOffsetRatio = -1.f;
        float RightOffsetRatio = -1.f;
        // ignore if stretch
        sf::Vector2f Size;
    };

    class WidgetCanvas : public Widget
    {
    public:
        DECLARE_CLASS_TYPE(WidgetCanvas, Widget)

        void Init()
        {
            SetPosition({0.f, 0.f});
            SetSize({
                static_cast<float>(Engine::EngineApplication::Get()->GetWindow().getSize().x),
                static_cast<float>(Engine::EngineApplication::Get()->GetWindow().getSize().y)
            });
        }

        void AddChild(Widget *child, int index) override
        {
            m_childOptions.insert({child, {
                .Size=child->GetSize()
            }});
            Widget::AddChild(child, index);
        }

        void RemoveChild(Widget *child) override
        {
            Widget::RemoveChild(child);
            m_childOptions.erase(child);
        }

        const CanvasChildOptions& GetChildOptions(const Widget* child) const
        {
            return m_childOptions.at(child);
        }

        void SetChildOptions(Widget* child, const CanvasChildOptions& Options) const
        {
            m_childOptions.at(child) = Options;
            SetChildSizeAndPositionFromOptions(child);
        }

    private:
        mutable std::map<const Widget*, CanvasChildOptions> m_childOptions;

        void UpdatePosition() override
        {
            auto children = GetChildren();
            for (auto& child : children)
            {
                SetChildSizeAndPositionFromOptions(child);
            }
        }

        void UpdateSize() override
        {
            auto children = GetChildren();
            for (auto& child : children)
            {
                SetChildSizeAndPositionFromOptions(child);
            }
        }

        void SetChildSizeAndPositionFromOptions(Widget* child) const
        {
            const auto& ChildOptions = GetChildOptions(child);
            sf::Vector2f ChildSize;
            sf::Vector2f ChildPosition;
            GetChildSizeAndPositionFromSize(ChildOptions, ChildSize, ChildPosition);
            child->SetSize(ChildSize);
            child->SetPosition(ChildPosition);
        }

        void GetChildSizeAndPositionFromSize(
            const CanvasChildOptions& ChildOptions,
            sf::Vector2f& ChildSize,
            sf::Vector2f& ChildPosition
        ) const
        {
            const auto TopOffset = GetOffsetFromFixedOrRatioAndCanvasSize(m_size.y, ChildOptions.TopOffset, ChildOptions.TopOffsetRatio);
            const auto BottomOffset = GetOffsetFromFixedOrRatioAndCanvasSize(m_size.y, ChildOptions.BottomOffset, ChildOptions.BottomOffsetRatio);
            const auto LeftOffset = GetOffsetFromFixedOrRatioAndCanvasSize(m_size.x, ChildOptions.LeftOffset, ChildOptions.LeftOffsetRatio);
            const auto RightOffset = GetOffsetFromFixedOrRatioAndCanvasSize(m_size.x, ChildOptions.RightOffset, ChildOptions.RightOffsetRatio);

            // TOP
            if (CanvasAnchors::TOP_LEFT == ChildOptions.Anchors)
            {
                auto ChildSizeX = (ChildOptions.Size.x / CanvasChildOptions::ReferenceCanvasWidth) * m_size.x;
                auto ChildSizeY = (ChildOptions.Size.y / CanvasChildOptions::ReferenceCanvasHeight) * m_size.y;
                ChildSize = {
                    std::min(ChildSizeX, m_size.x - (LeftOffset + RightOffset)),
                    std::min(ChildSizeY, m_size.y - (TopOffset + BottomOffset))
                };
                ChildPosition = {m_position.x + LeftOffset, m_position.y + TopOffset};
                return;
            }

            if (CanvasAnchors::TOP_CENTER == ChildOptions.Anchors)
            {
                auto ChildSizeX = (ChildOptions.Size.x / CanvasChildOptions::ReferenceCanvasWidth) * m_size.x;
                auto ChildSizeY = (ChildOptions.Size.y / CanvasChildOptions::ReferenceCanvasHeight) * m_size.y;
                ChildSize = {
                    std::min(ChildSizeX, m_size.x - (LeftOffset + RightOffset)),
                    std::min(ChildSizeY, m_size.y - (TopOffset + BottomOffset))
                };
                ChildPosition = {m_position.x + (LeftOffset - RightOffset) + (m_size.x / 2) - (ChildSize.x / 2), m_position.y + TopOffset};
                return;
            }

            if (CanvasAnchors::TOP_RIGHT == ChildOptions.Anchors)
            {
                auto ChildSizeX = (ChildOptions.Size.x / CanvasChildOptions::ReferenceCanvasWidth) * m_size.x;
                auto ChildSizeY = (ChildOptions.Size.y / CanvasChildOptions::ReferenceCanvasHeight) * m_size.y;
                ChildSize = {
                    std::min(ChildSizeX, m_size.x - (LeftOffset + RightOffset)),
                    std::min(ChildSizeY, m_size.y - (TopOffset + BottomOffset))
                };
                ChildPosition = {m_position.x + m_size.x - RightOffset - ChildSize.x, m_position.y + TopOffset};
                return;
            }

            // LEFT
            if (CanvasAnchors::CENTER_LEFT == ChildOptions.Anchors)
            {
                auto ChildSizeX = (ChildOptions.Size.x / CanvasChildOptions::ReferenceCanvasWidth) * m_size.x;
                auto ChildSizeY = (ChildOptions.Size.y / CanvasChildOptions::ReferenceCanvasHeight) * m_size.y;
                ChildSize = {
                    std::min(ChildSizeX, m_size.x - (LeftOffset + RightOffset)),
                    std::min(ChildSizeY, m_size.y - (TopOffset + BottomOffset))
                };
                ChildPosition = {m_position.x + LeftOffset, m_position.y + (TopOffset - BottomOffset) + (m_size.y / 2) - (ChildSize.y / 2)};
                return;
            }

            if (CanvasAnchors::CENTER == ChildOptions.Anchors)
            {
                auto ChildSizeX = (ChildOptions.Size.x / CanvasChildOptions::ReferenceCanvasWidth) * m_size.x;
                auto ChildSizeY = (ChildOptions.Size.y / CanvasChildOptions::ReferenceCanvasHeight) * m_size.y;
                ChildSize = {
                    std::min(ChildSizeX, m_size.x - (LeftOffset + RightOffset)),
                    std::min(ChildSizeY, m_size.y - (TopOffset + BottomOffset))
                };
                ChildPosition = {m_position.x + (LeftOffset - RightOffset) + (m_size.x / 2) - (ChildSize.x / 2), m_position.y + (TopOffset - BottomOffset) + (m_size.y / 2) - (ChildSize.y / 2)};
                return;
            }

            if (CanvasAnchors::CENTER_RIGHT == ChildOptions.Anchors)
            {
                auto ChildSizeX = (ChildOptions.Size.x / CanvasChildOptions::ReferenceCanvasWidth) * m_size.x;
                auto ChildSizeY = (ChildOptions.Size.y / CanvasChildOptions::ReferenceCanvasHeight) * m_size.y;
                ChildSize = {
                    std::min(ChildSizeX, m_size.x - (LeftOffset + RightOffset)),
                    std::min(ChildSizeY, m_size.y - (TopOffset + BottomOffset))
                };
                ChildPosition = {m_position.x + m_size.x - RightOffset - ChildSize.x, m_position.y + (TopOffset - BottomOffset) + (m_size.y / 2) - (ChildSize.y / 2)};
                return;
            }

            // BOTTOM
            if (CanvasAnchors::BOTTOM_LEFT == ChildOptions.Anchors)
            {
                auto ChildSizeX = (ChildOptions.Size.x / CanvasChildOptions::ReferenceCanvasWidth) * m_size.x;
                auto ChildSizeY = (ChildOptions.Size.y / CanvasChildOptions::ReferenceCanvasHeight) * m_size.y;
                ChildSize = {
                    std::min(ChildSizeX, m_size.x - (LeftOffset + RightOffset)),
                    std::min(ChildSizeY, m_size.y - (TopOffset + BottomOffset))
                };
                ChildPosition = {m_position.x + LeftOffset, m_position.y + m_size.y - BottomOffset - ChildSize.y};
                return;
            }

            if (CanvasAnchors::BOTTOM_CENTER == ChildOptions.Anchors)
            {
                auto ChildSizeX = (ChildOptions.Size.x / CanvasChildOptions::ReferenceCanvasWidth) * m_size.x;
                auto ChildSizeY = (ChildOptions.Size.y / CanvasChildOptions::ReferenceCanvasHeight) * m_size.y;
                ChildSize = {
                    std::min(ChildSizeX, m_size.x - (LeftOffset + RightOffset)),
                    std::min(ChildSizeY, m_size.y - (TopOffset + BottomOffset))
                };
                ChildPosition = {m_position.x + (LeftOffset - RightOffset) + (m_size.x / 2) - (ChildSize.x / 2), m_position.y + m_size.y - BottomOffset - ChildSize.y};
                return;
            }

            if (CanvasAnchors::BOTTOM_RIGHT == ChildOptions.Anchors)
            {
                auto ChildSizeX = (ChildOptions.Size.x / CanvasChildOptions::ReferenceCanvasWidth) * m_size.x;
                auto ChildSizeY = (ChildOptions.Size.y / CanvasChildOptions::ReferenceCanvasHeight) * m_size.y;
                ChildSize = {
                    std::min(ChildSizeX, m_size.x - (LeftOffset + RightOffset)),
                    std::min(ChildSizeY, m_size.y - (TopOffset + BottomOffset))
                };
                ChildPosition = {m_position.x + m_size.x - RightOffset - ChildSize.x, m_position.y + m_size.y - BottomOffset - ChildSize.y};
                return;
            }

            // HORIZONTAL STRETCH
            if (CanvasAnchors::HORIZONTAL_STRETCH_TOP == ChildOptions.Anchors)
            {
                auto ChildSizeY = (ChildOptions.Size.y / CanvasChildOptions::ReferenceCanvasHeight) * m_size.y;
                ChildSize = {
                    m_size.x - (LeftOffset + RightOffset),
                    std::min(ChildSizeY, m_size.y - (TopOffset + BottomOffset))
                };
                ChildPosition = {m_position.x + LeftOffset, m_position.y + TopOffset};
                return;
            }

            if (CanvasAnchors::HORIZONTAL_STRETCH_CENTER == ChildOptions.Anchors)
            {
                auto ChildSizeY = (ChildOptions.Size.y / CanvasChildOptions::ReferenceCanvasHeight) * m_size.y;
                ChildSize = {
                    m_size.x - (LeftOffset + RightOffset),
                    std::min(ChildSizeY, m_size.y - (TopOffset + BottomOffset))
                };
                ChildPosition = {m_position.x + LeftOffset, m_position.y + (TopOffset - BottomOffset) - (m_size.y / 2) - (ChildSize.y / 2)};
                return;
            }

            if (CanvasAnchors::HORIZONTAL_STRETCH_BOTTOM == ChildOptions.Anchors)
            {
                auto ChildSizeY = (ChildOptions.Size.y / CanvasChildOptions::ReferenceCanvasHeight) * m_size.y;
                ChildSize = {
                    m_size.x - (LeftOffset + RightOffset),
                    std::min(ChildSizeY, m_size.y - (TopOffset + BottomOffset))
                };
                ChildPosition = {m_position.x + LeftOffset, m_position.y + m_size.y - BottomOffset - ChildSize.y};
                return;
            }

            // VERTICAL STRETCH
            if (CanvasAnchors::VERTICAL_STRETCH_LEFT == ChildOptions.Anchors)
            {
                auto ChildSizeX = (ChildOptions.Size.x / CanvasChildOptions::ReferenceCanvasWidth) * m_size.x;
                ChildSize = {
                    std::min(ChildSizeX, m_size.x - (LeftOffset + RightOffset)),
                    m_size.y - (TopOffset + BottomOffset),
                };
                ChildPosition = {m_position.x + LeftOffset, m_position.y + m_size.y - BottomOffset - ChildSize.y};
                return;
            }

            if (CanvasAnchors::VERTICAL_STRETCH_CENTER == ChildOptions.Anchors)
            {
                auto ChildSizeX = (ChildOptions.Size.x / CanvasChildOptions::ReferenceCanvasWidth) * m_size.x;
                ChildSize = {
                    std::min(ChildSizeX, m_size.x - (LeftOffset + RightOffset)),
                    m_size.y - (TopOffset + BottomOffset),
                };
                ChildPosition = {m_position.x + (LeftOffset - RightOffset) + (m_size.x / 2) - (ChildSize.x / 2), m_position.y + m_size.y - BottomOffset - ChildSize.y};
                return;
            }

            if (CanvasAnchors::VERTICAL_STRETCH_RIGHT == ChildOptions.Anchors)
            {
                auto ChildSizeX = (ChildOptions.Size.x / CanvasChildOptions::ReferenceCanvasWidth) * m_size.x;
                ChildSize = {
                    std::min(ChildSizeX, m_size.x - (LeftOffset + RightOffset)),
                    m_size.y - (TopOffset + BottomOffset),
                };
                ChildPosition = {m_position.x + m_size.x - RightOffset - ChildSize.x, m_position.y + m_size.y - BottomOffset - ChildSize.y};
                return;
            }

            if (CanvasAnchors::STRETCH == ChildOptions.Anchors)
            {
                ChildSize = {
                    m_size.x - (LeftOffset + RightOffset),
                    m_size.y - (TopOffset + BottomOffset),
                };
                ChildPosition = {m_position.x, m_position.y};
                return;
            }
        }

        static float GetOffsetFromFixedOrRatioAndCanvasSize(float size, float offset, float ratio)
        {
            if (ratio >= 0.f)
                return size * ratio;
            return offset;
        }
    };
} // UI
