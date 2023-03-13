//
// Created by Flo on 22/02/2023.
//

#include "WidgetGrid.hpp"

namespace KrabEngine::UI {
    void WidgetGrid::Init(const sf::Vector2u &gridSize, const sf::Vector2f &gridSpacing, const sf::Vector2f &position, const sf::Vector2f &cellSize) {
        SetGridSize(gridSize);
        SetGridSpacing(gridSpacing);
        SetCellSize(cellSize);
        SetPosition(position);  // This will call UpdatePosition() and RecomputeSize()
    }

    void WidgetGrid::SetGridSize(const sf::Vector2u &gridSize) {
        m_gridSize = gridSize;
        RecomputeSize();
    }

    void WidgetGrid::SetGridSpacing(const sf::Vector2f &gridSpacing) {
        m_gridSpacing = gridSpacing;
        RecomputeSize();
    }

    void WidgetGrid::SetCellSize(const sf::Vector2f &cellSize) {
        m_cellSize = cellSize;
        RecomputeSize();
    }

    void WidgetGrid::UpdatePosition() {
        auto children = GetChildren();

        if(children.empty())
            return;

        auto position = GetPosition();
        int column = 0;
        int line = 0;

        for (auto& child : children) {

            auto childSize = child->GetSize();
            auto delta = sf::Vector2f(m_cellSize.x - childSize.x, m_cellSize.y - childSize.y);

            child->SetPosition(sf::Vector2f(position.x + delta.x / 2, position.y + delta.y / 2));
            position.x += m_cellSize.x + m_gridSpacing.x;
            column++;
            if(column == m_gridSize.x) {
                column = 0;
                position.x = GetPosition().x;
                position.y += m_cellSize.y + m_gridSpacing.y;
                line++;

                if(line == m_gridSize.y)
                    break;
            }
        }
    }

    void WidgetGrid::RecomputeSize() const {
        m_size = sf::Vector2f {
                m_cellSize.x * m_gridSize.x + m_gridSpacing.x * (m_gridSize.x - 1),
                m_cellSize.y * m_gridSize.y + m_gridSpacing.y * (m_gridSize.y - 1)
        };
    }

    void WidgetGrid::UpdateSize() {
        if (m_previousSize.x == 0 || m_previousSize.y == 0) return;

        auto RatioX = GetSize().x / m_previousSize.x;
        auto RatioY = GetSize().y / m_previousSize.y;
        if (RatioX == 1 && RatioY == 1) return;

        m_cellSize.x *= RatioX;
        m_cellSize.y *= RatioY;

        auto children = GetChildren();
        for (auto& child : children)
        {
            child->SetSize({
               child->GetSize().x * RatioX,
               child->GetSize().y * RatioY,
            });
        }
        UpdatePosition();
    }
} // UI