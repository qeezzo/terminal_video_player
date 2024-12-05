find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(test_suite
    test_player.cpp

    ../src/Video.cpp
    ../src/GrayFrame.cpp
    ../src/Player.cpp
    ../src/Watcher.cpp
)

target_link_libraries(test_suite PRIVATE GTest::GTest GTest::Main pthread ${opencv_LIBRARIES})
target_include_directories(test_suite PRIVATE ${opencv_INCLUDE_DIRS})

# Register tests
add_test(NAME player COMMAND test_suite)
