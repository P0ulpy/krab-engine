set(ENGINE_TARGET_NAME KrabEngine)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

add_library(${ENGINE_TARGET_NAME} STATIC "")

set_target_properties(${ENGINE_TARGET_NAME}
    PROPERTIES LINKER_LANGUAGE CXX
)

add_subdirectory(KrabEngine)
add_subdirectory(vendors)

target_link_libraries(${ENGINE_TARGET_NAME}
    PUBLIC sfml-system sfml-network sfml-graphics sfml-window sfml-audio
)

# Assets and Config copy
# set(CMAKE_SOURCE_ASSETS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Assets)
# set(CMAKE_RUNTIME_ASSETS_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Assets)
# set(CMAKE_SOURCE_CONFIG_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Configs)
# set(CMAKE_RUNTIME_CONFIG_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Configs)

# add_custom_command(
#     TARGET ${ENGINE_TARGET_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     ${CMAKE_SOURCE_ASSETS_DIRECTORY}
#     ${CMAKE_RUNTIME_ASSETS_DIRECTORY}
# )

# add_custom_command(
#     TARGET ${ENGINE_TARGET_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     ${CMAKE_SOURCE_CONFIG_DIRECTORY}
#     ${CMAKE_RUNTIME_CONFIG_DIRECTORY}
# )