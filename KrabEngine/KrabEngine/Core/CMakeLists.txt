target_sources(${ENGINE_TARGET_NAME}
    PUBLIC
        Core.hpp
        Time.hpp
        Time.cpp
        UUID.hpp
        UUID.cpp

        Application/ApplicationLayer.cpp
        Application/ApplicationLayer.hpp
        Application/EngineApplication.cpp
        Application/EngineApplication.hpp
        Application/LayerStack.hpp

        Camera/Camera.cpp
        Camera/Camera.hpp

        Components/CompositeComponent.cpp
        Components/CompositeComponent.hpp
        Components/Physics.cpp
        Components/Physics.hpp
        Components/SpriteRenderer.cpp
        Components/SpriteRenderer.hpp
        Components/Transform.cpp
        Components/Transform.hpp

        EntityPrefab/EntityPrefab.hpp

        Inputs/Binding.hpp
        Inputs/Connection.cpp
        Inputs/Connection.hpp
        Inputs/Connection.tpp
        Inputs/InputConfig.cpp
        Inputs/InputConfig.hpp
        Inputs/InputSignal.cpp
        Inputs/InputSignal.hpp
        Inputs/InputSignal.tpp
        Inputs/Observable.cpp
        Inputs/Observable.hpp
        Inputs/Observable.tpp
        Inputs/WindowEvents.cpp
        Inputs/WindowEvents.hpp

        Logger/ConsoleColor.hpp
        Logger/Logger.cpp
        Logger/Logger.hpp
        Logger/Logger.tpp

        Physics/Physics.cpp
        Physics/Physics.hpp

        ScenesSystem/ScenesSystem.cpp
        ScenesSystem/ScenesSystem.hpp
        ScenesSystem/ScenesSystem.tpp
        ScenesSystem/SceneInitializer/SceneInitializer.cpp
        ScenesSystem/SceneInitializer/SceneInitializer.hpp
        ScenesSystem/ScenesLayer/ScenesLayer.cpp
        ScenesSystem/ScenesLayer/ScenesLayer.hpp
)