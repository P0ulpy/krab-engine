//
// Created by Flo on 22/02/2023.
//

#pragma once

#include "../../Widget/Widget.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

namespace KrabEngine::UI {

        class SpriteWidget : public Widget {
        public:
            DECLARE_CLASS_TYPE(SpriteWidget, Widget)

            virtual void OnRender(sf::RenderTarget& renderTarget);

            void Init(const sf::Texture& texture, const sf::Vector2f& position);

            void SetPosition(const sf::Vector2f& position) override;
            void SetScale(const sf::Vector2f& scale);
            void SetSize(const sf::Vector2f& size) override;

        private:
            void RecomputeSize() const override;

        private:
            sf::Sprite m_sprite;
        };
} // UI
