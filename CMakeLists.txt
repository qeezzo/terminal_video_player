cmake_minimum_required(VERSION 3.5)
project(player)

set(CMAKE_CXX_STANDARD 20)

add_executable(player
    player.cpp

    Video.cpp
    GrayFrame.cpp
    Player.cpp
)

target_precompile_headers(player PRIVATE opencv.h)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
target_link_libraries( player ${OpenCV_LIBS})
