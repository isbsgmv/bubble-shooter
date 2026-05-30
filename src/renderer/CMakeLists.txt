find_package(SDL2 REQUIRED)

add_library(renderer
    src/sdl2_renderer.cpp
)

target_include_directories(renderer PUBLIC include)
target_include_directories(renderer PRIVATE src)
target_compile_features(renderer PRIVATE cxx_std_17)

target_link_libraries(renderer PUBLIC SDL2::SDL2)
target_link_libraries(renderer PRIVATE board)  # For Board class access

if(MSVC)
    target_compile_options(renderer PRIVATE /W4 /permissive-)
else()
    target_compile_options(renderer PRIVATE -Wall -Wextra -Wpedantic)
endif()
