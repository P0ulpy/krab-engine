//
// Created by Flo on 15/12/2022.
//

#ifndef PATHFINDER_ASSETLOADER_HPP
#define PATHFINDER_ASSETLOADER_HPP

#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Font.hpp"
#include <unordered_map>
#include "SFML/Graphics/Font.hpp"

namespace KrabEngine {

    template<class TAsset>
    class AssetLoader
    {
    public:
        static TAsset* StaticGetAsset(const std::string& path);

    private:
        static TAsset* StaticLoadAsset(const std::string& path);

        static std::unordered_map<std::string, TAsset> s_paths;
    };

    template <class TAsset>
    std::unordered_map<std::string, TAsset> AssetLoader<TAsset>::s_paths {};

    template<class TAsset>
    TAsset* AssetLoader<TAsset>::StaticGetAsset(const std::string &path)
    {
        if(!s_paths.contains(path))
            StaticLoadAsset(path);

        return &s_paths[path];
    }

    template<class TAsset>
    TAsset* AssetLoader<TAsset>::StaticLoadAsset(const std::string &path) { return nullptr; }

    template<>
    sf::Texture* AssetLoader<sf::Texture>::StaticLoadAsset(const std::string& path);

    template<>
    sf::Font* AssetLoader<sf::Font>::StaticLoadAsset(const std::string& path);

} // Engine

#endif //PATHFINDER_ASSETLOADER_HPP
