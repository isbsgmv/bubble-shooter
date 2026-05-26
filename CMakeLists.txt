cmake_minimum_required(VERSION 3.20)

project(game VERSION 0.1.0 LANGUAGES CXX)

include(CTest)

add_subdirectory(utils)
add_subdirectory(src/board)
add_subdirectory(src/projectile)

add_executable(game
    src/main.cpp
    src/game.cpp
)

target_include_directories(game PRIVATE src)
target_compile_features(game PRIVATE cxx_std_17)

target_link_libraries(game PRIVATE projectile board utils)

if(MSVC)
    target_compile_options(game PRIVATE /W4 /permissive-)
else()
    target_compile_options(game PRIVATE -Wall -Wextra -Wpedantic)
endif()
