find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(board_test
    board_test.cpp
)

target_link_libraries(board_test PRIVATE board GTest::gtest_main)

gtest_discover_tests(board_test)
