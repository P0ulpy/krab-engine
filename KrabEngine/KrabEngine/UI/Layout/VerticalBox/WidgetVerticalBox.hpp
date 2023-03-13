//
// Created by Flo on 22/02/2023.
//

#pragma once

#include "../../Widget/Widget.hpp"

namespace KrabEngine::UI {

        class WidgetVerticalBox : public Widget {
        public:
            DECLARE_CLASS_TYPE(WidgetVerticalBox, Widget)

            void Init(const sf::Vector2f& position, const float spacing);

            void SetSize(const sf::Vector2f& size) override
            {
                m_previousSize = m_size;
                Widget::SetSize(size);
            }
        private:
            sf::Vector2f m_previousSize {0, 0};

            void UpdatePosition() override;
            void RecomputeSize() const override;
            void UpdateSize() override;

            float m_spacing { 0.0f };
        };
} // UI
