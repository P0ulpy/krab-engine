target_sources(${ENGINE_TARGET_NAME}
    PUBLIC
        Component/Component.cpp
        Component/Component.hpp

        ComponentSystem/ComponentSystem.hpp
        ComponentSystem/ComponentSystem.tpp
        ComponentSystem/IComponentSystem.hpp

        EntitiesRegistry/EntitiesRegistry.cpp
        EntitiesRegistry/EntitiesRegistry.hpp
        EntitiesRegistry/EntitiesRegistry.tpp

        Entity/Entity.cpp
        Entity/Entity.hpp

        Handles/Handles.hpp

        Scene/Scene.cpp
        Scene/Scene.hpp
)