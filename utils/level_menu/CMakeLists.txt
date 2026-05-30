add_library(utils_level_menu STATIC
    src/level_menu.cpp
)

target_include_directories(utils_level_menu
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(utils_level_menu PRIVATE ${CMAKE_SOURCE_DIR}/src/include)

add_library(utils::level_menu ALIAS utils_level_menu)
