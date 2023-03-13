set(GAME_HEADERS
        src/main.hpp
)

set(GAME_SOURCES
        src/main.cpp
)

set(GAME_TARGET_NAME TestGame)

add_executable(${GAME_TARGET_NAME} ${GAME_HEADERS} ${GAME_SOURCES})

include_directories(${CMAKE_SOURCE_DIR}/KrabEngine)

target_link_libraries(${GAME_TARGET_NAME} ${EngineTargetName})

# Assets and Config copy
set(CMAKE_SOURCE_ASSETS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Assets)
set(CMAKE_RUNTIME_ASSETS_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Assets)
set(CMAKE_SOURCE_CONFIG_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Configs)
set(CMAKE_RUNTIME_CONFIG_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Configs)

add_custom_command(
        TARGET ${GAME_TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_ASSETS_DIRECTORY}
        ${CMAKE_RUNTIME_ASSETS_DIRECTORY}
)

add_custom_command(
        TARGET ${GAME_TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_CONFIG_DIRECTORY}
        ${CMAKE_RUNTIME_CONFIG_DIRECTORY}
)