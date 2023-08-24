target_sources(${ENGINE_TARGET_NAME}
    PUBLIC
        KrabEngine.hpp
)

add_subdirectory(AssetsSystems)
add_subdirectory(Core)
add_subdirectory(ECS)
add_subdirectory(Maths)
add_subdirectory(RTTI)
add_subdirectory(UI)
