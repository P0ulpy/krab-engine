include_directories(
    ${CMAKE_SOURCE_DIR}/KrabEngine/vendors/mINI/src
)

target_sources(${ENGINE_TARGET_NAME}
    PUBLIC
        mINI/src/mini/ini.h
)