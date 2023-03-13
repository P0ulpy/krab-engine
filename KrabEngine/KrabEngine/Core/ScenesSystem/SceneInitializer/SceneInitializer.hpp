//
// Created by Flo on 27/02/2023.
//

#pragma once

namespace KrabEngine
{
    class Scene;

    class SceneInitializer
    {
    public:
        SceneInitializer() = default;
        virtual ~SceneInitializer() = default;

        virtual void OnLoaded(Scene* scene) = 0;
        virtual void OnUnloaded(Scene* scene) = 0;
    };

} // Engine
