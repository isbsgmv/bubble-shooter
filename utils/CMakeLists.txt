add_library(utils STATIC)

# Add sources
set(UTILS_SOURCES
    bubble/src/bubble_color.cpp
)

# Add include directories
set(UTILS_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/bubble/include
)

# Target setup
target_sources(utils
    PRIVATE
        ${UTILS_SOURCES}
)

target_include_directories(utils
    PUBLIC
        ${UTILS_INCLUDES}
)

# Add tests
add_subdirectory(bubble/test)

