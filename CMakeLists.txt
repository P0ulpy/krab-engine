cmake_minimum_required(VERSION 3.21)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(krab-engine)

set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_FLAGS_RELEASE "-o3")

add_subdirectory(KrabEngine)
add_subdirectory(TestGame)