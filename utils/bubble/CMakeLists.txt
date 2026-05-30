add_library(utils_bubble STATIC
    src/bubble_color.cpp
)

target_include_directories(utils_bubble
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(COMPILE_TESTS)
    add_subdirectory(test)
endif()

add_library(utils::bubble ALIAS utils_bubble)
