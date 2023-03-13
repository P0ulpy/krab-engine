//
// Created by Flo on 22/02/2023.
//

#pragma once

#include "../../Widget/Widget.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

namespace KrabEngine::UI {

        class WidgetGrid : public Widget {
        public:
            DECLARE_CLASS_TYPE(WidgetGrid, Widget)

            void Init(const sf::Vector2u& gridSize, const sf::Vector2f& gridSpacing, const sf::Vector2f& position, const sf::Vector2f &cellSize = sf::Vector2f(70.0f, 70.0f));

            void SetGridSize(const sf::Vector2u& gridSize);
            void SetGridSpacing(const sf::Vector2f& gridSpacing);
            void SetCellSize(const sf::Vector2f& cellSize);

            [[nodiscard]] const sf::Vector2u& GetGridSize() const          { return m_gridSize; }
            [[nodiscard]] const sf::Vector2f& GetGridSpacing() const       { return m_gridSpacing; }
            [[nodiscard]] const sf::Vector2f& GetCellSize() const          { return m_cellSize; }


            void UpdatePosition() override;
            void UpdateSize() override;
            void RecomputeSize() const override;

            void SetSize(const sf::Vector2f& size) override
            {
                m_previousSize = m_size;
                Widget::SetSize(size);
            }
        private:
            sf::Vector2f m_previousSize {0, 0};
            sf::Vector2u m_gridSize { 0, 0 };
            sf::Vector2f m_gridSpacing { .0f, .0f };
            sf::Vector2f m_cellSize { .0f, .0f };
        };
} // UI
