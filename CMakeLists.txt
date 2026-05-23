cmake_minimum_required(VERSION 3.20)

project(game VERSION 0.1.0 LANGUAGES CXX)

add_executable(game
    src/main.cpp
    src/game.cpp
    src/board/board.cpp
)

target_include_directories(game PRIVATE src)
target_compile_features(game PRIVATE cxx_std_17)

if(MSVC)
    target_compile_options(game PRIVATE /W4 /permissive-)
else()
    target_compile_options(game PRIVATE -Wall -Wextra -Wpedantic)
endif()
