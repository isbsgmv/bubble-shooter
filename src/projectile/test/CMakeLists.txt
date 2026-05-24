find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(projectile_test
    projectile_test.cpp
)

target_link_libraries(projectile_test PRIVATE projectile board GTest::gtest_main)

gtest_discover_tests(projectile_test)

add_executable(projectile_trajectory_test
    projectile_trajectory_test.cpp
)

target_link_libraries(projectile_trajectory_test PRIVATE projectile board GTest::gtest_main)

gtest_discover_tests(projectile_trajectory_test)
