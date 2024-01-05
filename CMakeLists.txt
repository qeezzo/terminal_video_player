cmake_minimum_required(VERSION 3.5)
project(bad_apple)

set(CMAKE_CXX_STANDARD 20)

add_executable(bad_apple
    bad_apple.cpp
)

target_precompile_headers(bad_apple PRIVATE opencv.h)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
target_link_libraries( bad_apple ${OpenCV_LIBS} timer)
