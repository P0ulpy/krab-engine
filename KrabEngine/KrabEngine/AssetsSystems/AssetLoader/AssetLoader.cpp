//
// Created by Flo on 15/12/2022.
//

#include "AssetLoader.hpp"
#include "../../Core/Logger/Logger.hpp"

namespace KrabEngine
{
    template<>
    sf::Texture* AssetLoader<sf::Texture>::StaticLoadAsset(const std::string& path)
    {
        sf::Texture texture;
        
        if(!texture.loadFromFile(path))
        {
            Logger::Err("Failed to load texture from path: ", path);
            return nullptr;
        }

        s_paths[path] = texture;

        return &s_paths[path];
    }

    template<>
    sf::Font* AssetLoader<sf::Font>::StaticLoadAsset(const std::string& path)
    {
        sf::Font font;

        if(!font.loadFromFile(path))
        {
            Logger::Err("Failed to load font from path: ", path);
            return nullptr;
        }

        s_paths[path] = font;

        return &s_paths[path];
    }
} // KrabEngine