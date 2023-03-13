target_sources(${ENGINE_TARGET_NAME}
    PUBLIC
        KrabEngine.hpp
)

add_subdirectory(Core)
add_subdirectory(AssetsSystems)
add_subdirectory(UI)