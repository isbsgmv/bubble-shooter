cmake_minimum_required(VERSION 3.5)

# Aggregate utils interface that links the small utility libraries
add_library(utils INTERFACE)

# Add known utility submodules if they exist
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bubble/CMakeLists.txt)
    add_subdirectory(bubble)
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/level_menu/CMakeLists.txt)
    add_subdirectory(level_menu)
endif()

# Collect available util targets and link them to the interface target
set(_utils_libs)
if(TARGET utils_level_menu)
    list(APPEND _utils_libs utils_level_menu)
endif()
if(TARGET utils_bubble)
    list(APPEND _utils_libs utils_bubble)
endif()

if(_utils_libs)
    target_link_libraries(utils INTERFACE ${_utils_libs})
endif()

