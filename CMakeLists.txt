cmake_minimum_required(VERSION 3.5)
project(player)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(opencv REQUIRED opencv4)

add_executable(player
    src/player.cpp
    src/Video.cpp
    src/GrayFrame.cpp
    src/Player.cpp
    src/Watcher.cpp
)

target_precompile_headers(player PRIVATE src/opencv.h)

target_link_libraries(player PRIVATE ${opencv_LIBRARIES})

target_include_directories(player PRIVATE ${opencv_INCLUDE_DIRS})
