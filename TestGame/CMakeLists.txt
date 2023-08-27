set(GAME_TARGET_NAME TestGame)

add_executable(${GAME_TARGET_NAME} "")

target_include_directories(${GAME_TARGET_NAME}
    PUBLIC
        ${CMAKE_SOURCE_DIR}/KrabEngine
)

add_subdirectory(src)

target_link_libraries(${GAME_TARGET_NAME}
    PUBLIC
        KrabEngine
)

# Assets and Config copy
# set(CMAKE_SOURCE_ASSETS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Assets)
# set(CMAKE_RUNTIME_ASSETS_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Assets)
# set(CMAKE_SOURCE_CONFIG_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Configs)
# set(CMAKE_RUNTIME_CONFIG_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Configs)

# add_custom_command(
#         TARGET ${GAME_TARGET_NAME} POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E copy_directory
#         ${CMAKE_SOURCE_ASSETS_DIRECTORY}
#         ${CMAKE_RUNTIME_ASSETS_DIRECTORY}
# )

# add_custom_command(
#         TARGET ${GAME_TARGET_NAME} POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E copy_directory
#         ${CMAKE_SOURCE_CONFIG_DIRECTORY}
#         ${CMAKE_RUNTIME_CONFIG_DIRECTORY}
# )